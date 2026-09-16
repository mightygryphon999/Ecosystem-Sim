using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Numerics;
using System.Text.Json;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.ApplicationLifetimes;
using Avalonia.Markup.Xaml;
using Avalonia.Media;

namespace EcosystemSim;

public partial class App : Application
{
    public override void Initialize()
    {
        AvaloniaXamlLoader.Load(this);
    }

    public override void OnFrameworkInitializationCompleted()
    {
        if (ApplicationLifetime is IClassicDesktopStyleApplicationLifetime desktop)
        {
            desktop.MainWindow = new MainWindow();
        }

        base.OnFrameworkInitializationCompleted();
    }
}

public class EcosystemView : Control
{
    public Ecosystem? EcosystemData { get; set; }
    public float size = 5f;
    public bool simulationUIvisible = true;

    public override void Render(DrawingContext context)
    {
        if (!simulationUIvisible)
        {
            return;
        }

        base.Render(context);

        if (EcosystemData is null)
        {
            Debug.WriteLine("Ecosystem Data is null");
            return;
        }

        lock (EcosystemData.SyncRoot)
        {
            foreach (var water in EcosystemData.activeWater)
            {
                var brush = water.amountOfWater == 0
                    ? Brushes.Transparent
                    : water.amountOfWater <= 25
                        ? Brushes.LightBlue
                        : water.amountOfWater <= 75
                            ? Brushes.DodgerBlue
                            : Brushes.DarkBlue;

                var center = new Point(water.xPos, water.yPos);
                context.DrawEllipse(brush, null, center, size, size);
            }

            foreach (var food in EcosystemData.activeFood)
            {
                bool sprouted = food.age >= food.sproutingAge;
                var brush = sprouted ? Brushes.ForestGreen : Brushes.SaddleBrown;
                var center = new Point(food.xPos, food.yPos);
                context.DrawEllipse(brush, null, center, size - (sprouted ? 0f : 1f), size - (sprouted ? 0f : 1f));
            }

            foreach (var species in EcosystemData.activeSpecies)
            {
                var brush = species.predator ? Brushes.Red : Brushes.Black;
                var center = new Point(species.xPos, species.yPos);
                context.DrawEllipse(brush, null, center, size, size);
            }
        }
    }

    public void Refresh()
    {
        InvalidateVisual();
    }
}

public class Ecosystem
{
    private static readonly string start_time = DateTime.Now.ToString("yyyyMMdd_HHmmss");

    public readonly object SyncRoot = new();

    public List<Species> activeSpecies { get; set; } = new();
    public List<FoodSpecies> activeFood { get; set; } = new();
    public List<WaterZone> activeWater { get; set; } = new();

    public List<double> populationSizes = new();
    public List<double> foodSizes = new();
    public List<double> maleSpecies = new();
    public List<double> femaleSpecies = new();
    public List<double> sproutedPlants = new();
    public List<double> unSproutedPlants = new();
    public List<double> averageSpeedPrey = new();
    public List<double> averageReproductionAge = new();
    public List<double> averageEyeSight = new();

    public bool noRecording = true;
    public bool EnableDiagnostics { get; set; }
    public int simulationSteps;

    public void start()
    {
        Directory.CreateDirectory($"saves/{start_time}");
    }

    public void update()
    {
        UpdateTick();
    }

    public void UpdateTick()
    {
        lock (SyncRoot)
        {
            simulationSteps++;
            maleSpecies.Add(0);
            femaleSpecies.Add(0);

            var speed = new List<double>(activeSpecies.Count);
            var eyeSight = new List<double>(activeSpecies.Count);
            var reproductionAge = new List<double>(activeSpecies.Count);

            Log("==========Update==========");

            UpdateFoodInternal();

            for (int i = activeSpecies.Count - 1; i >= 0; i--)
            {
                Species species = activeSpecies[i];
                speed.Add(species.speed);
                eyeSight.Add(species.eyeSght);
                reproductionAge.Add(species.reproductiveAge);

                if (species.gender == 0)
                {
                    femaleSpecies[^1] += 1;
                }
                else
                {
                    maleSpecies[^1] += 1;
                }

                species.update();

                int wanted = species.wanted_resource();
                if (wanted == 0)
                {
                    bool worked = goToWater(species);
                    if (!worked)
                    {
                        if (species.predator)
                        {
                            goToSpeciesEat(species);
                        }
                        else
                        {
                            goToFood(species);
                        }
                    }
                }
                else if (wanted == 1)
                {
                    if (species.predator)
                    {
                        goToSpeciesEat(species);
                    }
                    else
                    {
                        goToFood(species);
                    }
                }
                else if (wanted == 2)
                {
                    int worked = goToSpecies(species);
                    if (worked == 0)
                    {
                        if (species.predator)
                        {
                            goToSpeciesEat(species);
                        }
                        else
                        {
                            goToFood(species);
                        }
                    }
                }
                else
                {
                    species.currentState = Species.State.nothing;
                }

                if (species.check_death())
                {
                    activeSpecies.RemoveAt(i);
                }
            }

            averageSpeedPrey.Add(speed.Count > 0 ? speed.Average() : 0);
            averageEyeSight.Add(eyeSight.Count > 0 ? eyeSight.Average() : 0);
            averageReproductionAge.Add(reproductionAge.Count > 0 ? reproductionAge.Average() : 0);

            populationSizes.Add(activeSpecies.Count);
            foodSizes.Add(activeFood.Count);

            foreach (var water in activeWater)
            {
                water.amountOfWater += 1f;
            }

            activeFood.RemoveAll(f => f.age >= f.maxLife);

            if (!noRecording)
            {
                saveToJson();
            }
        }
    }

    private void UpdateFoodInternal()
    {
        sproutedPlants.Add(0);
        unSproutedPlants.Add(0);

        var localFood = new List<FoodSpecies>(activeFood);
        var newFoods = new List<FoodSpecies>();

        foreach (var food in localFood)
        {
            food.age += 1f;
            if (food.age >= food.seedingAge)
            {
                food.seedingAge += food.originalSeedingAge;
                int spawnCount = food.amountOfFood + 1;

                for (int i = 0; i < spawnCount; i++)
                {
                    bool validPosition;
                    int attempts = 0;
                    FoodSpecies? candidate = null;

                    do
                    {
                        float x = Math.Clamp(food.xPos + Random.Shared.Next(-150, 150), 0, 1600);
                        float y = Math.Clamp(food.yPos + Random.Shared.Next(-150, 150), 0, 900);

                        candidate = new FoodSpecies(
                            1,
                            (int)x,
                            (int)y,
                            food.seedsAmount + Random.Shared.Next(-1, 2),
                            food.sproutingAge + Random.Shared.Next(-1, 2),
                            food.originalSeedingAge + Random.Shared.Next(-1, 2),
                            food.maxLife + Random.Shared.Next(-1, 2))
                        {
                            seedingAge = food.originalSeedingAge
                        };

                        validPosition = true;

                        foreach (var existing in localFood)
                        {
                            if (Vector2.Distance(new Vector2(existing.xPos, existing.yPos), new Vector2(x, y)) <= 20)
                            {
                                validPosition = false;
                                break;
                            }
                        }

                        if (validPosition)
                        {
                            foreach (var existingWater in activeWater)
                            {
                                if (Vector2.Distance(new Vector2(existingWater.xPos, existingWater.yPos), new Vector2(x, y)) <= 30)
                                {
                                    validPosition = false;
                                    break;
                                }
                            }
                        }

                        attempts++;
                    }
                    while (!validPosition && attempts < 20);

                    if (validPosition && candidate is not null)
                    {
                        newFoods.Add(candidate);
                    }
                }
            }

            if (food.age >= food.sproutingAge)
            {
                sproutedPlants[^1] += 1;
            }
            else
            {
                unSproutedPlants[^1] += 1;
            }
        }

        activeFood.AddRange(newFoods);
        activeFood.RemoveAll(f => f.age >= f.maxLife);
    }

    private void Log(string message)
    {
        if (EnableDiagnostics)
        {
            Console.WriteLine(message);
        }
    }

    public void saveToJson(string filename = "")
    {
        try
        {
            var options = new JsonSerializerOptions
            {
                WriteIndented = true,
                IncludeFields = true
            };

            if (string.IsNullOrEmpty(filename))
            {
                string timestamp = DateTime.Now.ToString("yyyyMMdd_HHmmss");
                filename = $"saves/{start_time}/date_{timestamp}.json";
            }

            string json = JsonSerializer.Serialize(this, options);
            File.WriteAllText(filename, json);
        }
        catch (Exception ex)
        {
            Console.WriteLine("Error saving json: " + ex);
        }
    }

    public int goToSpeciesEat(Species species)
    {
        Species? species1 = FindClosestOfTypeSpeciesPredators(species);
        if (species1 is null)
        {
            MoveRandom(species);
            return 0;
        }

        Vector2 currentPos = new(species.xPos, species.yPos);
        Vector2 targetPos = new(species1.xPos, species1.yPos);

        Vector2 direction = targetPos - currentPos;
        if (direction != Vector2.Zero)
        {
            Vector2 escapeDirection = Vector2.Normalize(direction);
            Vector2 escapeTarget = new(species1.xPos + escapeDirection.X * 100, species1.yPos + escapeDirection.Y * 100);
            species1.move_species(escapeTarget, true);
        }

        bool collided = species.move_species(targetPos, true);
        if (collided && species.gender == 0)
        {
            species.currentState = Species.State.eating;
            activeSpecies.Remove(species1);
            return 2;
        }

        return 1;
    }

    public int goToSpecies(Species species)
    {
        Species? species1 = FindClosestOfTypeSpecies(species);
        if (species1 is null)
        {
            MoveRandom(species);
            return 0;
        }

        Vector2 targetPos = new(species1.xPos, species1.yPos);
        bool collided = species.move_species(targetPos, false);

        if (collided && species.gender == 0)
        {
            species.currentState = Species.State.nothing;
            activeSpecies.Add(species.mate(species1));
            return 2;
        }

        return 1;
    }

    public bool goToFood(Species species)
    {
        FoodSpecies? food = FindClosestOfTypeFood(species);
        if (food is null)
        {
            MoveRandom(species);
            return false;
        }

        Vector2 targetPos = new(food.xPos, food.yPos);
        bool collided = species.move_species(targetPos, false);
        if (collided)
        {
            species.currentState = Species.State.eating;
            activeFood.Remove(food);
        }

        return true;
    }

    public bool goToWater(Species species)
    {
        WaterZone? water = FindClosestWaterZone(species);
        if (water is null)
        {
            MoveRandom(species);
            return false;
        }

        Vector2 targetPos = new(water.xPos, water.yPos);
        bool collided = species.move_species(targetPos, false);
        if (collided)
        {
            species.currentState = Species.State.drinking;
            species.drinkingWaterAmount = water.amountOfWater;
            water.amountOfWater = 0;
        }

        return true;
    }

    private static void MoveRandom(Species species)
    {
        (double x, double y) = RandomPointInCircle(100.0, new Vector2(species.xPos, species.yPos));
        species.move_species(new Vector2((float)x, (float)y), false);
    }

    private static (double x, double y) RandomPointInCircle(double radius, Vector2 offset)
    {
        double angle = Random.Shared.NextDouble() * MathF.PI * 2;
        double distance = Math.Sqrt(Random.Shared.NextDouble()) * radius;
        return (Math.Cos(angle) * distance + offset.X, Math.Sin(angle) * distance + offset.Y);
    }

    public FoodSpecies? FindClosestOfTypeFood(Species species)
    {
        return TargetingService.FindClosestFood(species, activeFood);
    }

    public Species? FindClosestOfTypeSpecies(Species species)
    {
        return TargetingService.FindClosestMate(species, activeSpecies);
    }

    public Species? FindClosestOfTypeSpeciesPredators(Species species)
    {
        return TargetingService.FindClosestPreyTarget(species, activeSpecies);
    }

    public WaterZone? FindClosestWaterZone(Species species)
    {
        return TargetingService.FindClosestWater(species, activeWater);
    }
}
