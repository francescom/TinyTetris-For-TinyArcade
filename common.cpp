


int getButtonState(int buttons,int pos) {
	return (buttons>>pos & 0x1);
}
