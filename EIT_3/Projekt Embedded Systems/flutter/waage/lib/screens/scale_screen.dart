import 'dart:async';
import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import 'package:permission_handler/permission_handler.dart';
import '../theme/app_theme.dart';
import '../widgets/device_list_tile.dart';
import '../widgets/weight_gauge.dart';
import '../services/weight_service.dart';

class ScaleScreen extends StatefulWidget {
  const ScaleScreen({super.key});

  @override
  State<ScaleScreen> createState() => _ScaleScreenState();
}

class _ScaleScreenState extends State<ScaleScreen> {
  final FlutterBluePlus flutterBlue = FlutterBluePlus();
  BluetoothDevice? device;
  StreamSubscription<List<int>>? notifSub;
  StreamSubscription<List<ScanResult>>? scanSub;

  DateTime? _lastUpdate;
  double? _lastShownWeight;

  List<ScanResult> scanResultsList = [];
  double? currentWeight;
  bool connecting = false;
  bool isScanning = false;

  final Guid serviceUuid = Guid("0000FF00-0000-1000-8000-00805F9B34FB");
  final Guid charUuid = Guid("0000FF01-0000-1000-8000-00805F9B34FB");

  @override
  void initState() {
    super.initState();
    checkBluetoothAndScan();
  }

  @override
  void dispose() {
    notifSub?.cancel();
    scanSub?.cancel();
    device?.disconnect();
    FlutterBluePlus.stopScan();
    super.dispose();
  }

  void checkBluetoothAndScan() async {
    var state = await FlutterBluePlus.adapterState.first;
    if (state != BluetoothAdapterState.on) {
      _showSnackBar('Bluetooth ist nicht aktiviert');
      return;
    }
    final ok = await _ensurePermissions();
    if (!ok) return;
    startScan();
  }

  Future<bool> _ensurePermissions() async {
    final permissions = <Permission>[
      Permission.bluetooth,
      Permission.bluetoothScan,
      Permission.bluetoothConnect,
      Permission.locationWhenInUse,
      Permission.location,
    ];

    final statuses = await permissions.request();
    bool granted = false;
    if (statuses[Permission.bluetoothScan]?.isGranted == true ||
        statuses[Permission.locationWhenInUse]?.isGranted == true ||
        statuses[Permission.location]?.isGranted == true) {
      granted = true;
    }

    if (!granted && mounted) {
      showDialog(
        context: context,
        builder: (context) => AlertDialog(
          backgroundColor: const Color(0xFF1E1E1E),
          title: const Text('Berechtigungen benötigt'),
          content: const Text('Bluetooth-/Standortberechtigungen werden zum Scannen benötigt.'),
          actions: [
            TextButton(
              onPressed: () => Navigator.of(context).pop(),
              child: const Text('OK'),
            ),
          ],
        ),
      );
    }

    return granted;
  }

  void startScan() {
    setState(() {
      connecting = true;
      isScanning = true;
      scanResultsList.clear();
    });

    scanSub = FlutterBluePlus.scanResults.listen((results) {
      setState(() {
        for (var r in results) {
          final idx = scanResultsList.indexWhere((e) => e.device.id == r.device.id);
          if (idx >= 0) {
            scanResultsList[idx] = r;
          } else {
            scanResultsList.add(r);
          }
        }
      });
    });

    try {
      FlutterBluePlus.startScan();
    } catch (e) {
      _showSnackBar('Scan-Fehler: $e');
    }

    Future.delayed(const Duration(seconds: 6), () {
      if (mounted) {
        setState(() => connecting = false);
        FlutterBluePlus.stopScan();
      }
    });
  }

  Future<void> connectDevice(BluetoothDevice d) async {
    device = d;
    try {
      setState(() => connecting = true);
      // Keine Timeout mehr - kontinuierliche Verbindung
      await device!.connect();
      scanSub?.cancel();

      // Reconnect bei Verbindungsverlust
      device!.state.listen((s) {
        if (s == BluetoothDeviceState.disconnected && mounted) {
          _showSnackBar('Verbindung verloren');
          setState(() => device = null);
          startScan();
        }
      });

      await discoverServices();
    } catch (e) {
      _showSnackBar('Connect-Fehler: $e');
      setState(() => device = null);
    } finally {
      if (mounted) setState(() => connecting = false);
    }
  }

  Future<void> discoverServices() async {
    if (device == null) return;
    try {
      List<BluetoothService> services = await device!.discoverServices();

      for (var s in services) {
        for (var c in s.characteristics) {
          if (c.properties.notify) {
            try {
              await c.setNotifyValue(true);
              notifSub = c.value.listen((value) {
                if (value.length >= 4) {
                  int raw = value[0] |
                      (value[1] << 8) |
                      (value[2] << 16) |
                      (value[3] << 24);
                  double weight = raw / 100.0;

                  if (c.uuid == charUuid) {
                    final now = DateTime.now();
                    final shouldUpdate = (_lastUpdate == null) ||
                        now.difference(_lastUpdate!).inMilliseconds > 200 ||
                        (_lastShownWeight == null) ||
                        (weight - _lastShownWeight!).abs() >= 1.0;
                    if (shouldUpdate) {
                      _lastUpdate = now;
                      _lastShownWeight = weight;
                      if (mounted) {
                        setState(() => currentWeight = weight);
                        // Update central weight notifier so follower can react
                        WeightService.setWeight(weight);
                      }
                    }
                  }
                }
              });
            } catch (e) {
              print('Error setting up notifications: $e');
            }
          }
        }
      }
    } catch (e) {
      _showSnackBar('Service-Fehler: $e');
    }
  }

  void _showSnackBar(String message) {
    ScaffoldMessenger.of(context).showSnackBar(SnackBar(content: Text(message)));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: device == null ? _buildScanView() : _buildWeightView(),
    );
  }

  Widget _buildScanView() {
    return SingleChildScrollView(
      child: Padding(
        padding: const EdgeInsets.all(24),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.stretch,
          children: [
            const SizedBox(height: 24),
            if (connecting)
              const Center(child: CircularProgressIndicator())
            else
              ElevatedButton.icon(
                onPressed: startScan,
                icon: const Icon(Icons.bluetooth_searching),
                label: const Text('Nach Waage scannen'),
              ),
            const SizedBox(height: 32),
            if (scanResultsList.isNotEmpty)
              Column(
                crossAxisAlignment: CrossAxisAlignment.start,
                children: [
                  Text(
                    'Verfügbare Geräte',
                    style: Theme.of(context).textTheme.titleLarge?.copyWith(
                          color: AppTheme.lightGreen,
                          fontWeight: FontWeight.bold,
                        ),
                  ),
                  const SizedBox(height: 12),
                  ...scanResultsList.map((r) => DeviceListTile(
                        result: r,
                        onTap: () async {
                          await connectDevice(r.device);
                        },
                      )),
                ],
              )
            else if (!connecting)
              Center(
                child: Column(
                  children: [
                    Icon(Icons.bluetooth_disabled,
                        size: 64, color: AppTheme.green.withOpacity(0.5)),
                    const SizedBox(height: 12),
                    const Text('Keine Geräte gefunden'),
                  ],
                ),
              ),
          ],
        ),
      ),
    );
  }

  Widget _buildWeightView() {
    return Stack(
      children: [
        // Hintergrund: Gauge + Device-Name
        Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              SizedBox(
                width: 320,
                height: 320,
                child: WeightGauge(
                  currentWeight: currentWeight,
                  maxWeight: 5000, // 5kg
                  isRecipeMode: false,
                ),
              ),
              const SizedBox(height: 24),
              Text(
                device?.name ?? 'Verbunden',
                style: Theme.of(context)
                    .textTheme
                    .bodyMedium
                    ?.copyWith(color: AppTheme.green),
              ),
            ],
          ),
        ),

        // Vordergrund: Grammanzeige über dem Gauge
        Center(
          child: Padding(
            padding: const EdgeInsets.only(top: 60),
            child: Text(
              currentWeight == null ? "..." : "${currentWeight!.toStringAsFixed(1)} g",
              style: const TextStyle(
                fontSize: 48,
                fontWeight: FontWeight.bold,
                color: AppTheme.lightGreen,
              ),
            ),
          ),
        ),

        // Close button
        Positioned(
          top: 16,
          right: 16,
          child: FloatingActionButton(
            mini: true,
            backgroundColor: AppTheme.red,
            onPressed: () {
              device?.disconnect();
              setState(() {
                device = null;
                currentWeight = null;
              });
              startScan();
            },
            child: const Icon(Icons.close),
          ),
        ),
      ],
    );
  }
}
