extends Node2D

@onready var sim: GDParticleSimulation = $GDParticleSimulation

func _ready():
	return 0

func _on_button_pressed() -> void:
	sim.reset_simulation()
