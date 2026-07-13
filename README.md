# Stormwind occlusion fix

Patcher module for the World of Warcraft 3.3.5a (build 12340) client. It disables
the broken first occluder-table entry used in Stormwind (portal bug) by changing its map ID
from Eastern Kingdoms (`0`) to the unused value `9999`.
