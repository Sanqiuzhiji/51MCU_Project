param(
    [Parameter(Mandatory = $true)]
    [string]$Firmware,
    [string]$Port,
    [switch]$DetectOnly
)

$ErrorActionPreference = 'Stop'

function Get-Ch340Ports {
    $usbRoot = 'Registry::HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Enum\USB'
    $ports = @()
    $activePorts = @([System.IO.Ports.SerialPort]::GetPortNames() |
            ForEach-Object { $_.ToUpperInvariant() })

    if (-not (Test-Path -LiteralPath $usbRoot)) {
        return $ports
    }

    $deviceRoots = Get-ChildItem -LiteralPath $usbRoot -ErrorAction SilentlyContinue |
        Where-Object { $_.PSChildName -match '^VID_1A86&PID_(7523|5523)' }

    foreach ($deviceRoot in $deviceRoots) {
        foreach ($instance in Get-ChildItem -LiteralPath $deviceRoot.PSPath -ErrorAction SilentlyContinue) {
            $parametersPath = Join-Path $instance.PSPath 'Device Parameters'
            $portName = (Get-ItemProperty -LiteralPath $parametersPath -Name PortName `
                    -ErrorAction SilentlyContinue).PortName

            if ($portName -match '^COM\d+$' -and
                $activePorts -contains $portName.ToUpperInvariant()) {
                $ports += $portName.ToUpperInvariant()
            }
        }
    }

    return @($ports | Sort-Object -Unique)
}

function Test-Command([string]$Command) {
    if ([string]::IsNullOrWhiteSpace($Command)) {
        return $false
    }
    if (Test-Path -LiteralPath $Command) {
        return $true
    }
    return $null -ne (Get-Command $Command -ErrorAction SilentlyContinue)
}

if ([string]::IsNullOrWhiteSpace($Port)) {
    $Port = $env:STC_PORT
}

if ([string]::IsNullOrWhiteSpace($Port)) {
    $detectedPorts = @(Get-Ch340Ports)
    if ($detectedPorts.Count -eq 0) {
        throw 'No CH340 serial port found. Connect the board or set STC_PORT (for example, COM9).'
    }
    if ($detectedPorts.Count -gt 1) {
        $portList = $detectedPorts -join ', '
        throw "Multiple CH340 serial ports found: $portList. Set STC_PORT and retry."
    }
    $Port = $detectedPorts[0]
}

$Port = $Port.ToUpperInvariant()
if ($Port -notmatch '^COM\d+$') {
    throw "Invalid serial port '$Port'. Expected a value such as COM9."
}

Write-Host "Using serial port: $Port"

if ($DetectOnly) {
    exit 0
}

$resolvedFirmware = (Resolve-Path -LiteralPath $Firmware).Path
$projectRoot = Split-Path $PSScriptRoot -Parent
$stcgalCandidates = @(
    $env:STCGAL_EXE,
    (Join-Path $projectRoot '.venv\Scripts\stcgal.exe'),
    'stcgal'
)

$stcgal = $stcgalCandidates | Where-Object { Test-Command $_ } | Select-Object -First 1
if ([string]::IsNullOrWhiteSpace($stcgal)) {
    throw 'stcgal was not found. Install it or set STCGAL_EXE to its executable path.'
}

& $stcgal -p $Port $resolvedFirmware
if ($LASTEXITCODE -ne 0) {
    throw "stcgal failed with exit code $LASTEXITCODE."
}
