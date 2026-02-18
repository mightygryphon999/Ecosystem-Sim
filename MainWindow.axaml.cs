using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Avalonia;
using Avalonia.Controls;
using Avalonia.Controls.Shapes;
using Avalonia.Input;
using Avalonia.Media;
using Avalonia.Threading;

namespace EcosystemSim;

public partial class MainWindow : Window
{
    private readonly SimulationSettings _settings = new();

    private Ecosystem _ecosystem = new();
    private CancellationTokenSource _simulationCts = new();
    private Task? _simulationTask;
    private readonly DispatcherTimer _uiTimer;

    private SimulationState _state = SimulationState.NotStarted;
    private bool _worldSeeded;

    private Button _startPauseButton = null!;
    private Button _resetButton = null!;
    private Button _applySettingsButton = null!;
    private TextBox _stepLimitBox = null!;
    private TextBox _initialPreyBox = null!;
    private TextBox _initialPredatorBox = null!;
    private TextBox _initialFoodBox = null!;
    private TextBox _tickDelayBox = null!;
    private TextBox _renderDelayBox = null!;
    private TextBlock _validationText = null!;

    private TextBlock _statusText = null!;
    private TextBlock _stepText = null!;
    private TextBlock _speciesText = null!;
    private TextBlock _foodText = null!;
    private TextBlock _waterText = null!;
    private ProgressBar _simulationProgressBar = null!;

    private ComboBox _graphSelector = null!;
    private Canvas _graphCanvas = null!;

    public MainWindow()
    {
        InitializeComponent();
        BindControls();

        KeyDown += OnKeyDown;
        Closing += (_, _) => _simulationCts.Cancel();

        InitializeSimulation();

        _uiTimer = new DispatcherTimer
        {
            Interval = TimeSpan.FromMilliseconds(_settings.RenderIntervalMs)
        };
        _uiTimer.Tick += (_, _) => UpdateUiFrame();
        _uiTimer.Start();
    }

    private void BindControls()
    {
        _startPauseButton = this.FindControl<Button>("StartPauseButton")!;
        _resetButton = this.FindControl<Button>("ResetButton")!;
        _applySettingsButton = this.FindControl<Button>("ApplySettingsButton")!;
        _stepLimitBox = this.FindControl<TextBox>("StepLimitBox")!;
        _initialPreyBox = this.FindControl<TextBox>("InitialPreyBox")!;
        _initialPredatorBox = this.FindControl<TextBox>("InitialPredatorBox")!;
        _initialFoodBox = this.FindControl<TextBox>("InitialFoodBox")!;
        _tickDelayBox = this.FindControl<TextBox>("TickDelayBox")!;
        _renderDelayBox = this.FindControl<TextBox>("RenderDelayBox")!;
        _validationText = this.FindControl<TextBlock>("ValidationText")!;

        _statusText = this.FindControl<TextBlock>("StatusText")!;
        _stepText = this.FindControl<TextBlock>("StepText")!;
        _speciesText = this.FindControl<TextBlock>("SpeciesText")!;
        _foodText = this.FindControl<TextBlock>("FoodText")!;
        _waterText = this.FindControl<TextBlock>("WaterText")!;
        _simulationProgressBar = this.FindControl<ProgressBar>("SimulationProgressBar")!;

        _graphSelector = this.FindControl<ComboBox>("GraphSelector")!;
        _graphCanvas = this.FindControl<Canvas>("GraphCanvas")!;

        _startPauseButton.Click += OnStartPauseClicked;
        _resetButton.Click += OnResetClicked;
        _applySettingsButton.Click += (_, _) =>
        {
            if (ApplySettingsFromUi())
            {
                UpdateControlState();
                UpdateUiFrame();
            }
        };
        _graphSelector.SelectionChanged += (_, _) => DrawSelectedGraph();
    }

    private void InitializeSimulation()
    {
        _ecosystem = new Ecosystem();
        _ecosystem.start();
        EcosystemCanvas.EcosystemData = _ecosystem;

        SyncSettingsToUi();
        UpdateControlState();
        UpdateUiFrame();
    }

    private async void OnStartPauseClicked(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        switch (_state)
        {
            case SimulationState.NotStarted:
            case SimulationState.Completed:
            case SimulationState.Cancelled:
                await StartSimulationAsync();
                break;
            case SimulationState.Running:
                PauseSimulation();
                break;
            case SimulationState.Paused:
                ResumeSimulation();
                break;
        }
    }

    private async void OnResetClicked(object? sender, Avalonia.Interactivity.RoutedEventArgs e)
    {
        await ResetSimulationAsync();
    }

    private async Task StartSimulationAsync()
    {
        if (_simulationTask is { IsCompleted: false })
        {
            return;
        }

        if (!ApplySettingsFromUi())
        {
            return;
        }

        if (!_worldSeeded)
        {
            SeedWorld();
        }

        _simulationCts = new CancellationTokenSource();
        _state = SimulationState.Running;
        UpdateControlState();

        _simulationTask = Task.Run(() => SimulationLoopAsync(_simulationCts.Token));
        await Task.CompletedTask;
    }

    private async Task ResetSimulationAsync()
    {
        _simulationCts.Cancel();

        if (_simulationTask is { IsCompleted: false })
        {
            try
            {
                await _simulationTask;
            }
            catch (OperationCanceledException)
            {
            }
        }

        _simulationTask = null;
        _simulationCts = new CancellationTokenSource();
        _state = SimulationState.NotStarted;
        _worldSeeded = false;
        _validationText.Text = string.Empty;

        InitializeSimulation();
    }

    private async Task SimulationLoopAsync(CancellationToken token)
    {
        try
        {
            while (!token.IsCancellationRequested)
            {
                if (_state == SimulationState.Paused)
                {
                    await Task.Delay(20, token);
                    continue;
                }

                if (_state != SimulationState.Running)
                {
                    break;
                }

                bool shouldComplete;
                lock (_ecosystem.SyncRoot)
                {
                    shouldComplete = _ecosystem.simulationSteps >= _settings.MaxSimulationSteps || _ecosystem.activeSpecies.Count == 0;
                }

                if (shouldComplete)
                {
                    _state = SimulationState.Completed;
                    break;
                }

                _ecosystem.UpdateTick();

                if (_settings.TickDelayMs > 0)
                {
                    await Task.Delay(_settings.TickDelayMs, token);
                }
                else
                {
                    await Task.Yield();
                }
            }
        }
        catch (OperationCanceledException)
        {
            _state = SimulationState.Cancelled;
        }
        finally
        {
            await Dispatcher.UIThread.InvokeAsync(() =>
            {
                UpdateControlState();
                UpdateUiFrame();
            });
        }
    }

    private void PauseSimulation()
    {
        if (_state != SimulationState.Running)
        {
            return;
        }

        _state = SimulationState.Paused;
        UpdateControlState();
    }

    private void ResumeSimulation()
    {
        if (_state != SimulationState.Paused)
        {
            return;
        }

        _state = SimulationState.Running;
        UpdateControlState();
    }

    private void OnKeyDown(object? sender, KeyEventArgs e)
    {
        if (e.Key != Key.Space)
        {
            return;
        }

        if (_state == SimulationState.Running)
        {
            PauseSimulation();
        }
        else if (_state == SimulationState.Paused)
        {
            ResumeSimulation();
        }
    }

    private void SeedWorld()
    {
        lock (_ecosystem.SyncRoot)
        {
            Spawner.SeedInitialWorld(_ecosystem, _settings, 1600, 900);
        }

        _worldSeeded = true;
    }

    private bool ApplySettingsFromUi()
    {
        if (!TryParseNonNegative(_stepLimitBox.Text, 1, out int maxSteps) ||
            !TryParseNonNegative(_initialPreyBox.Text, 0, out int prey) ||
            !TryParseNonNegative(_initialPredatorBox.Text, 0, out int predators) ||
            !TryParseNonNegative(_initialFoodBox.Text, 0, out int food) ||
            !TryParseNonNegative(_tickDelayBox.Text, 0, out int tickDelay) ||
            !TryParseNonNegative(_renderDelayBox.Text, 16, out int renderDelay))
        {
            _validationText.Text = "Values must be integers. Steps >= 1, Render interval >= 16, others >= 0.";
            return false;
        }

        _settings.MaxSimulationSteps = maxSteps;
        _settings.InitialPrey = prey;
        _settings.InitialPredators = predators;
        _settings.InitialFood = food;
        _settings.TickDelayMs = tickDelay;
        _settings.RenderIntervalMs = renderDelay;

        _uiTimer.Interval = TimeSpan.FromMilliseconds(_settings.RenderIntervalMs);
        _validationText.Text = string.Empty;
        return true;
    }

    private static bool TryParseNonNegative(string? raw, int min, out int value)
    {
        if (int.TryParse(raw, out int parsed) && parsed >= min)
        {
            value = parsed;
            return true;
        }

        value = 0;
        return false;
    }

    private void SyncSettingsToUi()
    {
        _stepLimitBox.Text = _settings.MaxSimulationSteps.ToString();
        _initialPreyBox.Text = _settings.InitialPrey.ToString();
        _initialPredatorBox.Text = _settings.InitialPredators.ToString();
        _initialFoodBox.Text = _settings.InitialFood.ToString();
        _tickDelayBox.Text = _settings.TickDelayMs.ToString();
        _renderDelayBox.Text = _settings.RenderIntervalMs.ToString();
    }

    private void UpdateControlState()
    {
        _statusText.Text = $"Status: {_state}";

        _startPauseButton.Content = _state switch
        {
            SimulationState.NotStarted => "Start Simulation",
            SimulationState.Running => "Pause Simulation",
            SimulationState.Paused => "Resume Simulation",
            SimulationState.Completed => "Restart Simulation",
            SimulationState.Cancelled => "Start Simulation",
            _ => "Start Simulation"
        };

        bool settingsEditable = _state is SimulationState.NotStarted or SimulationState.Completed or SimulationState.Cancelled;

        _stepLimitBox.IsEnabled = settingsEditable;
        _initialPreyBox.IsEnabled = settingsEditable;
        _initialPredatorBox.IsEnabled = settingsEditable;
        _initialFoodBox.IsEnabled = settingsEditable;
        _tickDelayBox.IsEnabled = settingsEditable;
        _renderDelayBox.IsEnabled = settingsEditable;
        _applySettingsButton.IsEnabled = settingsEditable;
    }

    private void UpdateUiFrame()
    {
        SimulationSnapshot snapshot = SimulationMetrics.CreateSnapshot(_ecosystem, _settings.MaxSimulationSteps);

        _stepText.Text = $"Step: {snapshot.Step} / {_settings.MaxSimulationSteps}";
        _speciesText.Text = $"Species: {snapshot.SpeciesCount}";
        _foodText.Text = $"Food: {snapshot.FoodCount}";
        _waterText.Text = $"Water Tiles: {snapshot.WaterCount}";
        _simulationProgressBar.Value = snapshot.Progress;

        EcosystemCanvas.Refresh();
        DrawSelectedGraph();
    }

    private void DrawSelectedGraph()
    {
        int selectedIndex = _graphSelector.SelectedIndex;
        if (selectedIndex == 4)
        {
            _graphCanvas.Children.Clear();
            return;
        }

        List<List<double>> data;
        List<IBrush> colors;
        List<string> names;

        lock (_ecosystem.SyncRoot)
        {
            switch (selectedIndex)
            {
                case 1:
                    data = new List<List<double>> { new(_ecosystem.femaleSpecies), new(_ecosystem.maleSpecies) };
                    colors = new List<IBrush> { Brushes.IndianRed, Brushes.Black };
                    names = new List<string> { "Female", "Male" };
                    break;
                case 2:
                    data = new List<List<double>> { new(_ecosystem.sproutedPlants), new(_ecosystem.unSproutedPlants) };
                    colors = new List<IBrush> { Brushes.ForestGreen, Brushes.SaddleBrown };
                    names = new List<string> { "Sprouted", "Unsprouted" };
                    break;
                case 3:
                    data = new List<List<double>>
                    {
                        _ecosystem.averageEyeSight.Select(v => v / 10d).ToList(),
                        new(_ecosystem.averageReproductionAge),
                        new(_ecosystem.averageSpeedPrey)
                    };
                    colors = new List<IBrush> { Brushes.SeaGreen, Brushes.DodgerBlue, Brushes.Crimson };
                    names = new List<string> { "Eye Sight / 10", "Reproduction", "Speed" };
                    break;
                default:
                    data = new List<List<double>> { new(_ecosystem.populationSizes), new(_ecosystem.foodSizes) };
                    colors = new List<IBrush> { Brushes.Red, Brushes.Green };
                    names = new List<string> { "Population", "Food" };
                    break;
            }
        }

        DrawLineGraph(data, colors, names);
    }

    private void DrawLineGraph(IReadOnlyList<List<double>> datas, IReadOnlyList<IBrush> colors, IReadOnlyList<string> names)
    {
        _graphCanvas.Children.Clear();

        double width = Math.Max(1, _graphCanvas.Bounds.Width - 20);
        double height = Math.Max(1, _graphCanvas.Bounds.Height - 30);
        if (width <= 1 || height <= 1)
        {
            return;
        }

        double yMax = datas.SelectMany(d => d).DefaultIfEmpty(1).Max();
        yMax = Math.Max(1, yMax * 1.1);

        for (int j = 0; j < datas.Count; j++)
        {
            List<double> data = datas[j];
            if (data.Count == 0)
            {
                continue;
            }

            var polyline = new Polyline
            {
                Stroke = colors[j],
                StrokeThickness = 2
            };

            double xStep = data.Count > 1 ? width / (data.Count - 1) : 0;
            double yScale = height / yMax;

            for (int i = 0; i < data.Count; i++)
            {
                double x = 10 + (data.Count > 1 ? i * xStep : 0);
                double y = 10 + height - (data[i] * yScale);
                polyline.Points.Add(new Point(x, y));
            }

            _graphCanvas.Children.Add(polyline);
        }

        for (int i = 0; i < names.Count; i++)
        {
            var rect = new Rectangle
            {
                Width = 14,
                Height = 14,
                Fill = colors[i]
            };
            Canvas.SetLeft(rect, 12);
            Canvas.SetTop(rect, 12 + i * 18);
            _graphCanvas.Children.Add(rect);

            var text = new TextBlock
            {
                Text = names[i],
                FontSize = 12,
                Foreground = Brushes.Black
            };
            Canvas.SetLeft(text, 32);
            Canvas.SetTop(text, 10 + i * 18);
            _graphCanvas.Children.Add(text);
        }
    }
}
