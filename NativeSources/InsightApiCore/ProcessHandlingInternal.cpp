#include <windows.h>

#include "ProcessHandling.h"
#include <vector>
#include <string>
#include <memory>
#include <map>
#include <detours.h>

// don't forget the header that contains the class def for each entry we will be spawning/tracking.
#include "PS_ProcessInformation.h"
#include "Utility.h"
#include "ClientHandling.h"


