#include "plugin.hpp"

#include <sys/stat.h>

#if ARCH_WIN
	#include <windows.h>
	#include <direct.h>
	#define mkdir(_dir, _perms) _mkdir(_dir)
#else
	#include <dlfcn.h>
#endif

using namespace std;

Plugin *pluginInstance;

void init(Plugin *p) {
	pluginInstance = p;

	// Add all Models defined throughout the plugin
	p->addModel(modelGVerbModule);
	p->addModel(modelAudioInterface16);
	p->addModel(modelCV0to10Module);
	p->addModel(modelCVS0to10Module);
	p->addModel(modelCV5to5Module);
	p->addModel(modelCVMmtModule);
	p->addModel(modelCVTglModule);
	p->addModel(modelPianoRollModule);
	p->addModel(modelDuckModule);
	p->addModel(modelSEQAdapterModule);
	p->addModel(modelSyncModule);
	p->addModel(modelPolyNosModule);
	// Any other plugin initialization may go here.
	// As an alternative, consider lazy-loading assets and lookup tables when your module is created to reduce startup times of Rack.
}
