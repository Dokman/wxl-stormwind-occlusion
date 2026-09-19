# Stormwind occlusion fix

Patcher module for wxl-core 1.1 and the World of Warcraft 3.3.5a (build 12340) client. It disables
the broken first occluder-table entry used in Stormwind (portal bug) by changing its map ID
from Eastern Kingdoms (`0`) to the unused value `9999`.

Copy `src/patcher/StormwindOcclusion.cpp` into the matching `src/patcher` directory of
`wxl-core` 1.1, then build `wxl-patcher` normally. The source layout mirrors wxl-core so the
repository can also be overlaid directly onto a wxl-core checkout.
