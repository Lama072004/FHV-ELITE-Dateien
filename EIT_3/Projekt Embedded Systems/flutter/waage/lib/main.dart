import 'package:flutter/material.dart';
import 'package:hive_flutter/hive_flutter.dart';
import 'theme/app_theme.dart';
import 'screens/scale_screen.dart';
import 'screens/recipes_screen.dart';
import 'models/recipe.dart';

void main() async {
  WidgetsFlutterBinding.ensureInitialized();
  await Hive.initFlutter();

  // Register Hive adapters (recipe.g.dart is a part file of recipe.dart)
  if (!Hive.isAdapterRegistered(0)) {
    Hive.registerAdapter(RecipeAdapter());
  }
  if (!Hive.isAdapterRegistered(1)) {
    Hive.registerAdapter(IngredientAdapter());
  }

  // Open typed box
  await Hive.openBox<Recipe>('recipes');

  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Waage',
      theme: AppTheme.darkTheme,
      home: const MainScreen(),
      debugShowCheckedModeBanner: false,
    );
  }
}

class MainScreen extends StatefulWidget {
  const MainScreen({super.key});
  @override
  State<MainScreen> createState() => _MainScreenState();
}

class _MainScreenState extends State<MainScreen> {
  int _selectedIndex = 0;
  static const List<Widget> _pages = <Widget>[
    ScaleScreen(),
    RecipesScreen(),
  ];

  void _onItemTapped(int index) => setState(() => _selectedIndex = index);

  @override
  Widget build(BuildContext context) {
    final isLandscape =
        MediaQuery.of(context).orientation == Orientation.landscape;

    if (isLandscape) {
      // Landscape: Navigation auf der rechten Seite mit gleichmäßigem Abstand
      return Scaffold(
        body: Row(
          children: [
            Expanded(
              child: IndexedStack(index: _selectedIndex, children: _pages),
            ),
            SizedBox(
              width: 80,
              child: Column(
                mainAxisAlignment: MainAxisAlignment.spaceEvenly,
                children: [
                  _buildNavButton(
                    icon: Icons.scale,
                    label: 'Waage',
                    index: 0,
                  ),
                  _buildNavButton(
                    icon: Icons.menu_book,
                    label: 'Rezepte',
                    index: 1,
                  ),
                ],
              ),
            ),
          ],
        ),
      );
    } else {
      // Portrait: Navigation unten
      return Scaffold(
        body: IndexedStack(index: _selectedIndex, children: _pages),
        bottomNavigationBar: NavigationBar(
          selectedIndex: _selectedIndex,
          onDestinationSelected: _onItemTapped,
          destinations: const [
            NavigationDestination(icon: Icon(Icons.scale), label: 'Waage'),
            NavigationDestination(icon: Icon(Icons.menu_book), label: 'Rezepte'),
          ],
        ),
      );
    }
  }

  Widget _buildNavButton({
    required IconData icon,
    required String label,
    required int index,
  }) {
    final isSelected = _selectedIndex == index;
    return GestureDetector(
      onTap: () => _onItemTapped(index),
      child: Column(
        mainAxisSize: MainAxisSize.min,
        children: [
          Container(
            decoration: BoxDecoration(
              color: isSelected ? AppTheme.lightGreen : Colors.transparent,
              shape: BoxShape.circle,
            ),
            padding: const EdgeInsets.all(12),
            child: Icon(
              icon,
              color: isSelected ? AppTheme.darkGreen : AppTheme.green,
              size: 28,
            ),
          ),
          const SizedBox(height: 4),
          Text(
            label,
            style: TextStyle(
              fontSize: 10,
              color: isSelected ? AppTheme.lightGreen : AppTheme.green,
              fontWeight: isSelected ? FontWeight.bold : FontWeight.normal,
            ),
          ),
        ],
      ),
    );
  }
}
