struct ButtonState {
	bool down;
	bool change;
};

enum {
	BUTTON_UP,
	BUTTON_DOWN,
	//BUTTON_LEFT,
	//BUTTON_RIGHT,
	BUTTON_W,
	BUTTON_S,

	BUTTON_COUNT, // use as last item in case we need to add more buttons
};

struct Input {
	ButtonState buttons[BUTTON_COUNT];
};