name: waage
description: "BLE Waage App für Android"
publish_to: 'none'

version: 1.0.0+1

environment:
  sdk: ">=3.9.2 <4.0.0"

dependencies:
  flutter:
    sdk: flutter

  # Pin to 1.x to keep the older, compatible API used in this app.
  flutter_blue_plus: ^1.7.0
  permission_handler: ^12.0.1

  cupertino_icons: ^1.0.8

  # Für Rezept-Import/Export
  file_picker: ^6.1.0
  share_plus: ^7.2.0
  path_provider: ^2.1.0
  crypto: ^3.0.3
  convert: ^3.1.1

  # Für die Datenbank
  hive: ^2.2.3
  hive_flutter: ^1.1.0

dev_dependencies:
  flutter_test:
    sdk: flutter
  flutter_lints: ^6.0.0
  flutter_launcher_icons: ^0.13.1
  hive_generator: ^2.0.0
  build_runner: ^2.4.0

flutter:
  uses-material-design: true

  assets:
    - assets/icon.png

flutter_launcher_icons:
  android:
    notification_icon: "icon_notification"
    adaptive_icon_background: "#386641"
    adaptive_icon_foreground: "assets/icon.png"
  ios:
    image_path: "assets/icon.png"
    image_path_dark: "assets/icon.png"

tools/run_build_runner.sh:
#!/usr/bin/env bash
set -e

echo "Running flutter clean..."
flutter clean

echo "Running flutter pub get..."
flutter pub get

echo "Running build_runner (recommended via dart run)..."
dart run build_runner build --delete-conflicting-outputs

echo "build_runner finished."