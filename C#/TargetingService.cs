using System;
using System.Collections.Generic;
using System.Numerics;

namespace EcosystemSim;

public static class TargetingService
{
    public static FoodSpecies? FindClosestFood(Species species, List<FoodSpecies> foods)
    {
        float closestDistance = float.MaxValue;
        FoodSpecies? match = null;

        foreach (FoodSpecies food in foods)
        {
            float distance = Vector2.Distance(new Vector2(food.xPos, food.yPos), new Vector2(species.xPos, species.yPos));
            if (distance < closestDistance && food.age >= food.sproutingAge)
            {
                match = food;
                closestDistance = distance;
            }
        }

        return closestDistance > species.eyeSght ? null : match;
    }

    public static Species? FindClosestMate(Species species, List<Species> speciesList)
    {
        float closestDistance = float.MaxValue;
        Species? match = null;

        foreach (Species candidate in speciesList)
        {
            if (ReferenceEquals(candidate, species))
            {
                continue;
            }

            float distance = Vector2.Distance(new Vector2(candidate.xPos, candidate.yPos), new Vector2(species.xPos, species.yPos));
            if (distance < closestDistance && candidate.gender != species.gender && candidate.predator == species.predator)
            {
                match = candidate;
                closestDistance = distance;
            }
        }

        return closestDistance > species.eyeSght ? null : match;
    }

    public static Species? FindClosestPreyTarget(Species species, List<Species> speciesList)
    {
        float closestDistance = float.MaxValue;
        Species? match = null;

        foreach (Species candidate in speciesList)
        {
            if (ReferenceEquals(candidate, species))
            {
                continue;
            }

            float distance = Vector2.Distance(new Vector2(candidate.xPos, candidate.yPos), new Vector2(species.xPos, species.yPos));
            if (distance < closestDistance && candidate.gender != species.gender)
            {
                match = candidate;
                closestDistance = distance;
            }
        }

        return closestDistance > species.eyeSght ? null : match;
    }

    public static WaterZone? FindClosestWater(Species species, List<WaterZone> waterZones)
    {
        float closestDistance = float.MaxValue;
        WaterZone? match = null;

        foreach (WaterZone water in waterZones)
        {
            float distance = Vector2.Distance(new Vector2(water.xPos, water.yPos), new Vector2(species.xPos, species.yPos));
            if (distance < closestDistance && water.amountOfWater > 0)
            {
                match = water;
                closestDistance = distance;
            }
        }

        return closestDistance > species.eyeSght ? null : match;
    }
}
