#include "sdpch.h"
#include "RenderContext.h"

namespace Strand {

    void RenderContext::UploadFrameConstants() {
        if (FrameConstantBuffer) {
            FrameConstantBuffer->SetData(&FrameData, sizeof(FrameConstants));
        }
    }

    void RenderContext::UploadRenderObjects() {
		//TODO SORT OBJECTS
        uint32_t count = Queue.GetCount();
        if (count == 0 || !ObjectSSBO) return;

        size_t uploadSize = count * sizeof(RenderObject);
        ObjectSSBO->SetData(Queue.GetData().data(), static_cast<uint32_t>(uploadSize));
    }

    void RenderContext::Reset() {
        Queue.Clear();
    }

} // namespace Strand