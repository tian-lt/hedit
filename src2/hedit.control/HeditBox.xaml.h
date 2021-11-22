#pragma once

#include "Document.h"
#include "HeditBox.g.h"

namespace Hedit::Control {
	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class HeditBox sealed {
	public:
		HeditBox();

	public:
		property Document^ Doc {
			Document^ get() {
				return _doc;
			}
		}

	private:
		Document^ _doc;
	};
}
