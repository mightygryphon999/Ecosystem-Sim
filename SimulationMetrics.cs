using System;

namespace EcosystemSim;

public sealed record SimulationSnapshot(
    int Step,
    int SpeciesCount,
    int FoodCount,
    int WaterCount,
    double Progress
);

public static class SimulationMetrics
{
    public static SimulationSnapshot CreateSnapshot(Ecosystem ecosystem, int maxSteps)
    {
        lock (ecosystem.SyncRoot)
        {
            double progress = maxSteps <= 0
                ? 0
                : Math.Clamp(ecosystem.simulationSteps / (double)maxSteps, 0, 1);

            return new SimulationSnapshot(
                ecosystem.simulationSteps,
                ecosystem.activeSpecies.Count,
                ecosystem.activeFood.Count,
                ecosystem.activeWater.Count,
                progress);
        }
    }
}
