# Stormwind occlusion fix

Extension DLL for wxl-core 1.1 and the World of Warcraft 3.3.5a (build 12340) client. It disables
the broken first occluder-table entry used in Stormwind (portal bug) by changing its map ID
from Eastern Kingdoms (`0`) to the unused value `9999` when the extension loads.

Clone or copy this repository as `extensions/wxl-stormwind-occlusion` inside a `wxl-core` 1.1
checkout, then build the `wxl-stormwind-occlusion` target. Deploy the resulting DLL as
`Extensions/wxl-stormwind-occlusion/wxl-stormwind-occlusion.dll` next to the game client.
