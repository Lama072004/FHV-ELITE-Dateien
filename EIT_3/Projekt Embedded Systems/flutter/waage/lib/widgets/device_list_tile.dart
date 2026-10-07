import 'package:flutter/material.dart';
import 'package:flutter_blue_plus/flutter_blue_plus.dart';
import '../theme/app_theme.dart';

class DeviceListTile extends StatelessWidget {
  final ScanResult result;
  final VoidCallback onTap;

  const DeviceListTile({
    required this.result,
    required this.onTap,
    super.key,
  });

  @override
  Widget build(BuildContext context) {
    final name = result.advertisementData.localName.isNotEmpty
        ? result.advertisementData.localName
        : (result.device.name.isNotEmpty ? result.device.name : result.device.id.id);

    return Padding(
      padding: const EdgeInsets.only(bottom: 8),
      child: Material(
        color: const Color(0xFF2A2A2A),
        borderRadius: BorderRadius.circular(12),
        child: InkWell(
          onTap: onTap,
          borderRadius: BorderRadius.circular(12),
          child: Padding(
            padding: const EdgeInsets.all(16),
            child: Row(
              children: [
                const Icon(Icons.scale, color: AppTheme.lightGreen),
                const SizedBox(width: 16),
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        name,
                        style: Theme.of(context).textTheme.titleMedium?.copyWith(
                              fontWeight: FontWeight.bold,
                            ),
                      ),
                      Text(
                        'RSSI: ${result.rssi}',
                        style: Theme.of(context).textTheme.bodySmall?.copyWith(
                              color: AppTheme.green,
                            ),
                      ),
                    ],
                  ),
                ),
                const Icon(Icons.arrow_forward, color: AppTheme.green),
              ],
            ),
          ),
        ),
      ),
    );
  }
}
