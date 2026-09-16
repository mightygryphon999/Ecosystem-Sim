using System;
using System.Numerics;

namespace EcosystemSim;

public static class Spawner
{
    public static void SeedInitialWorld(Ecosystem ecosystem, SimulationSettings settings, int worldWidth, int worldHeight)
    {
        for (int i = 0; i < settings.InitialPrey; i++)
        {
            var species = new Species("5:500:1:2500:75:0:100", "5:500:0:2500:75:0:100", Random.Shared.Next(0, worldWidth), Random.Shared.Next(0, worldHeight));
            species.inherit_genes();
            ecosystem.activeSpecies.Add(species);
        }

        for (int i = 0; i < settings.InitialPredators; i++)
        {
            var species = new Species("5:500:1:2500:25:1:100", "5:500:0:2500:25:1:100", Random.Shared.Next(0, worldWidth), Random.Shared.Next(0, worldHeight));
            species.inherit_genes();
            ecosystem.activeSpecies.Add(species);
        }

        for (int i = 0; i < settings.InitialFood; i++)
        {
            ecosystem.activeFood.Add(new FoodSpecies(1, Random.Shared.Next(0, worldWidth), Random.Shared.Next(0, worldHeight), Random.Shared.Next(1, 4), 50, 500 + Random.Shared.Next(-50, 51), 1000 + Random.Shared.Next(-50, 51))
            {
                seedingAge = 500
            });
        }

        AddWaterCluster(ecosystem, 250, 250, new Vector2(1200, 500));
        AddWaterCluster(ecosystem, 250, 250, new Vector2(600, 350));
        AddWaterCluster(ecosystem, 50, 50, new Vector2(100, 500));
    }

    private static void AddWaterCluster(Ecosystem ecosystem, int count, double radius, Vector2 center)
    {
        for (int i = 0; i < count; i++)
        {
            (double x, double y) = RandomPointInCircle(radius, center);
            ecosystem.activeWater.Add(new WaterZone(1, (int)x, (int)y));
        }
    }

    private static (double x, double y) RandomPointInCircle(double radius, Vector2 offset)
    {
        double angle = Random.Shared.NextDouble() * MathF.PI * 2;
        double distance = Math.Sqrt(Random.Shared.NextDouble()) * radius;
        return (Math.Cos(angle) * distance + offset.X, Math.Sin(angle) * distance + offset.Y);
    }
}
