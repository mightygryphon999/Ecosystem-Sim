namespace EcosystemSim;

public enum SimulationState
{
    NotStarted,
    Running,
    Paused,
    Completed,
    Cancelled
}

public sealed class SimulationSettings
{
    public int MaxSimulationSteps { get; set; } = 30000;
    public int InitialPrey { get; set; } = 150;
    public int InitialPredators { get; set; } = 45;
    public int InitialFood { get; set; } = 300;
    public int TickDelayMs { get; set; } = 1;
    public int RenderIntervalMs { get; set; } = 75;
}
