#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>

#include "GDSimulation.h"
#include "2_GDSimulation.h"
#include "2_GDSoftBody.h"
#include "2_GDCollider.h"

using namespace godot;

void initialize_extension(ModuleInitializationLevel level) {
    if (level == MODULE_INITIALIZATION_LEVEL_SCENE) {
        ClassDB::register_abstract_class<GDSimulation>();
        ClassDB::register_class<GDParticleSimulation>();
        ClassDB::register_class<GDSoftBodySimulation>();
        ClassDB::register_class<GDSoftBodySimulationBtn>();
        ClassDB::register_abstract_class<GDSoftBody>();
        ClassDB::register_class<GDSoftBodyCustom>();
        ClassDB::register_class<GDSoftBodyTriange>();
        ClassDB::register_class<GDSoftBodySquare>();
        ClassDB::register_class<GDSoftBodyPolygone>();
        ClassDB::register_class<GDParticle>();
        ClassDB::register_class<GDConstraint>();

        ClassDB::register_class<GDSimulation_2>();
        ClassDB::register_class<GDSoftBody_2>();
        ClassDB::register_class<GDCollider>();
    }
}

void uninitialize_extension(ModuleInitializationLevel level) {
    // nothing
}

extern "C" {
    GDExtensionBool GDE_EXPORT sim_library_init(
        GDExtensionInterfaceGetProcAddress p_get_proc_address,
        GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization
    ) {
        GDExtensionBinding::InitObject init_obj(
            p_get_proc_address, p_library, r_initialization
        );
        init_obj.register_initializer(initialize_extension);
        init_obj.register_terminator(uninitialize_extension);
        init_obj.set_minimum_library_initialization_level(
            MODULE_INITIALIZATION_LEVEL_SCENE);

        return init_obj.init();
    }
}
