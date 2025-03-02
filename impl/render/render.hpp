namespace n_render {
	class c_render {
	public:
		auto imgui() -> bool;
		auto hijack() -> bool;
		auto render() -> bool;
	};
} static n_render::c_render* render = new n_render::c_render();