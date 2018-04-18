all:
	@if ! [ -d build ]; then mkdir build; fi
	@cd build && cmake .. && make

test: all
	$(shell read -p "Input model path: " model_path; ./build/bin/homework-3 $$model_path)