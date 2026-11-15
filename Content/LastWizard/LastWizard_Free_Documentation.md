# Last Wizard Free — Unreal Engine 5.8

Modular fantasy room and corridor, Knight with four animations, fireplace, table, wall torches, windows, Blue banners, rug, candlestick, philosopher's stone, Reliquary chest and Gold Chalice. Includes narrow wall-infill UV variants and a script-free animated candle material.

Open Maps/L_LastWizard_Free_Overview to inspect labelled asset and animation stations. Maps/L_LastWizard_Free_Demo reproduces the approved Unity URP scene: 10x10 m room, 15x3.4 m corridor, two 5 m wall rows and a 10 m ceiling. The approved scene has seven corridor banners, including one over the portal. Its two lights named Spot Light are Point lights and are preserved as such. The static door leaf is 5 degrees ajar, matching the saved source; it has no interaction or opening script.

Play uses the one in-pack BP_LastWizard_Free_FlyGameMode and the engine DefaultPawn. WASD/mouse fly; geometry collision is enabled. No gameplay event-graph logic, runtime scripts or third-party plugins are included. Knight uses Animation Asset mode with Idle playing/looping. Supplied clips: Idle, Walk, Attack, Death. Custom skeleton; no Run or Jump invented.

Original Complete assets retain their paths and bytes. New changed assemblies use Free names/paths. Door parts were imported separately from the original Complete FBX to reproduce the saved leaf pose; Complete door assets were not edited. HalfUV wall meshes keep full 5 m geometry and are shown at half X scale in the Demo. The candle blueprint uses a non-blocking, two-sided additive material with camera-facing World Position Offset, sway, height variation, texture distortion and flicker. Its mesh has collision data, but the decorative effect Blueprint disables collision by design. Torch fire uses the existing Free Niagara system with a two-second warm-up. Initial rendering/captures should allow shaders and simulation to warm up.

PBR maps: Base Color, DirectX Normal import convention, ARM (AO R / Roughness G / Metallic B). Lighting is realtime; no light bake. Source placements are retained; Unreal light/exposure values are adjusted to its rendering units. Engine Niagara/ACL content may be referenced; no optional project plugins are enabled in the delivery. Scale is centimetres. No Epic sample/demo content included.

The Overview presents all shipped asset assemblies and the skeletal mesh/four animation clips, with labels and grounded stations. Candle is enlarged for the display and ceiling is upright to show its underside; their asset defaults are unchanged. The Demo and Overview both have a PlayerStart and fly GameMode.

Validation results, measurements and known limitations are supplied in the external QA/technical specification handoff. This is an asset pack, not a packaged game. Windows UE5.8 editor/PIE is the validation target; other OSs, packaged builds and mobile are not certified by this work. AI-generated content: Yes (confirmed by the artist 2026-10-29); the source Last Wizard pack was made with an AI-assisted workflow and finished by artists.

## Measured content

23 unique meshes (22 static + 1 skeletal), 40,454 vertices / 44,352 triangles at LOD0. No added LODs. 21 Blueprints:20 asset/effect assemblies and one fly GameMode. 4 parent materials + 21 material instances; 61 textures. Texture dimensions: 4 x 4x4, 1 x 256x256, 3 x 512x512, 12 x 1024x1024, 41 x 2048x2048.

Knight: 68 bones, custom skeleton, 20 default physics bodies; four animation sequences (Idle 4.433 s, Walk 3.300 s, Attack 2.167 s, Death 2.800 s), root motion disabled. No Run/Jump. Two maps and 25 labelled overview stations; static door pose is 5 degrees ajar.

Collision includes authored simple shapes and complex-as-simple on the Free arch, door frame and cropped-UV wall meshes. The Free arch variant removes the source hull that sealed the opening. The candle mesh has complex collision data while its effect Blueprint is non-colliding. Rotated wooden trim instances compensate Unreal's non-uniform parent scaling to match the source scene.

Unreal lighting calibration adds three unshadowed corridor-floor fills and one unshadowed door fill (25 cd each) and reduces the very close table/candle light to 8 cd. Source light positions/colors remain otherwise preserved; these changes compensate engine attenuation. Temporal antialiasing is enabled.
