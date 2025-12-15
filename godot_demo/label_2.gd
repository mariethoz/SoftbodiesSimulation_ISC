extends Label

var i: int = 0;

func _on_button_2_pressed() -> void:
	i += 1;
	text = "Nb : " + str(i);


func _on_button_pressed() -> void:
	i = 0;
	text = "Nbd";
