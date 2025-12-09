extends Node2D


@export var default: GDSoftBodyPolygone
@onready var gd_soft_body_simulation_btn: GDSoftBodySimulationBtn = $GDSoftBodySimulationBtn

func _on_button_2_pressed() -> void:
	gd_soft_body_simulation_btn.add_body(default)
