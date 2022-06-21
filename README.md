# LevelEditor
## Directx 11 C++

Current implimentation feature the following operation for sculpting terrain

Can load a Bitmap Image as base Height map with Terrain Dimension matched to that of imported image.
## Sculpt Mode
1.	[x] **RAISE**     : Raise the Terrain portion that comes under the brush radius.
2.	[x] **LOWER**     : Lower the Terrain portion that comes under the brush radius.
3.	[x] **FLATTEN**   : Flatten the Terrain portion that comes under the brush radius with height of picked point.
4.	[x] **SMOOTH**    : Smooths the Terrain portion that comes under the brush radius.
5.	[x] **ALPHAMAP**  : Load a Heightmap to be used as brush and Sculpt directly on Terrain.
6.	[x] **NOISE**     : Add Noise with configurable parameters to terrain part under the brush.
7.	[x] **HOLES**     : Create a Hole in Terrain(Caves).

Brush Radius is configurable.

## Mode Under Development
1.  [ ] **TODO**      : Integrate Terrain Color Layers for Grass, Snow, Rocks and more as well as splat maps for paths and roads.

## Future Development
2.  [ ] **TODO**      : LOD and Geomipmaping.
3.  [ ] **TODO**      : Integrate Road-Bridges network.
4.  [ ] **TODO**      : Forward Plus Lighting model.
5.  [ ] **TODO**      : Animations.
6.  [ ] **TODO**      : Collision Detection and Object Selection.
7.  [ ] **TODO**      : Integrate Level-Map Export File Option.
