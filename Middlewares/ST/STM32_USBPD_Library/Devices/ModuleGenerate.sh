#!/bin/bash
cd ../../../../../__INTERNAL__tools/tools/_ModuleGenerator_USBPD/
perl ./_Tools/ModuleGeneration.pl -i Devices -debug 0
read -n1 -r -p "Press any key to continue..." key