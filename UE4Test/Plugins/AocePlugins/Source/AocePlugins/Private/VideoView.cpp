#include "VideoView.hpp"
#include "Core/Public/Core.h"
namespace aoce {

	VideoView::VideoView(GpuType gpu_type) {
//		this->gpuType = gpuType;
//#if WIN32
//		if (gpuType == GpuType::other) {
//			this->gpuType = GpuType::cuda;
//		}
//#elif __ANDROID__
//		this->gpuType = GpuType::vulkan;
//#endif
		// 生成一张执行图
		graph = std::unique_ptr<IPipeGraph>(
			getPipeGraphFactory(gpuType)->createGraph());
		auto* layerFactory = getLayerFactory(gpuType);
		inputLayer = std::unique_ptr<IInputLayer>(layerFactory->createInput());
		outputLayer = std::unique_ptr<IOutputLayer>(layerFactory->createOutput());
		// edgeBlurLayer = std::unique_ptr<IEdgeBlurBlendLayer>(createEdgeBlurBlendLayer(gpuType));
 #if WIN32
		outputLayer->updateParamet({ false,true });
 #endif
		//EdgeBlurBlendParamet eparamet = {};
		//eparamet.blurRadius = 10;
		//eparamet.thresholdSobel = 0.5;
		//eparamet.mix = 10;
		//edgeBlurLayer->updateParamet(eparamet);

		yuv2rgbLayer =
			std::unique_ptr<IYUVLayer>(layerFactory->createYUV2RGBA());
		// 链接图
		graph->addNode(inputLayer.get())
			->addNode(yuv2rgbLayer.get())
			// ->addNode(edgeBlurLayer.get())
			->addNode(outputLayer.get());
	}

	VideoView::~VideoView() {}

	void VideoView::runFrame(const VideoFrame& frame, bool special) {
		if (getYuvIndex(frame.videoType) >= 0) {
			yuv2rgbLayer->getLayer()->setVisable(true);
			if (yuv2rgbLayer->getParamet().type != frame.videoType) {
				yuv2rgbLayer->updateParamet({ frame.videoType, special });
			}
		}
		else {
			yuv2rgbLayer->getLayer()->setVisable(false);
		}
		inputLayer->inputCpuData(frame);
		graph->run();
	}

}  // namespace aoce