# Assets

The asset files here are not used by the game's executable and are therefore not required.
They were converted to c-style byte arrays using this site: https://notisrac.github.io/FileToCArray/.
Because of this they are directly integrated into the source.

## Themes

Wyrmsweeper has some sort of 'Theme system' with the `ITheme` class. This class declares some functions that return
Textures and Fonts used by the game.

### How to create themes

1. Create a sprite sheet with the same layout as the classic theme [sprite sheet](classic_theme/sprite_sheet.png) (The
   size's of the sprite sheet does not have to be the same but each tile must have the same width and height).
2. Convert them into a c-style array using the previously mentioned site
    - <u>Format:</u> 32bit RGBA
    - <u>Data type: int</u>
    - <u>Endian:</u> Big-Endian (might have to be changed according to your pc)
3. Create a class inheriting from the `ITheme` interface and override all the necessary functions (for reference
   see [classic_theme.cpp](../src/themes/classic_theme.cpp))
4. Change the line 51 in [wyrmsweeper.cpp](../src/app/wyrmsweeper.cpp)
   to `_currentTheme = std::make_unique<YourTheme>();`

As of now there is no screen to change the theme in game as there is no second theme.