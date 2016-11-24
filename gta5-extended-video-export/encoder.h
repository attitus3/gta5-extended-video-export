#pragma once

#pragma comment(lib, "avcodec.lib")
#pragma comment(lib, "avdevice.lib")
#pragma comment(lib, "avfilter.lib")
#pragma comment(lib, "avformat.lib")
#pragma comment(lib, "avutil.lib")

#include <Windows.h>
#include <mfidl.h>
extern "C" {
#include <libavcodec\avcodec.h>
#include <libavformat\avformat.h>
#include <libavutil\imgutils.h>
#include <libavutil\opt.h>
}

namespace Encoder {
	class Session {
	public:
		AVCodec *codec;
		AVCodecContext *codecContext = NULL;
		AVOutputFormat *oformat;
		AVFormatContext *fmtContext;
		AVFrame *frame;
		AVStream *stream;
		UINT width;
		UINT height;
		UINT framerate;
		UINT pts = 0;
		AVPixelFormat pixelFormat;
		AVPixelFormat inputFramePixelFormat;
		//LPWSTR *outputDir;
		//LPWSTR *outputFile;
		FILE *file;

		Session();

		~Session();

		HRESULT createContext(LPCSTR filepath, UINT width, UINT height, AVPixelFormat inputFramePixelFormat, UINT fps_num, UINT fps_den);

		HRESULT writeFrame(BYTE *pData, int length);

		HRESULT endSession();

		HRESULT convertNV12toYUV420P(const BYTE* image_src, BYTE* image_dst,
			int image_width, int image_height) {
			BYTE* p = image_dst;
			memcpy(p, image_src, image_width * image_height * 3 / 2);
			const BYTE* pNV = image_src + image_width * image_height;
			BYTE* pU = p + image_width * image_height;
			BYTE* pV = p + image_width * image_height + ((image_width * image_height) >> 2);
			for (int i = 0; i < (image_width * image_height) / 2; i++) {
				if ((i % 2) == 0) *pU++ = *(pNV + i);
				else *pV++ = *(pNV + i);
			}
			return S_OK;
		}
	};

	HRESULT createSession(IMFTransform* pTransform);
	HRESULT getSession(IMFTransform* pTransform, Session** ppEncoder);
	HRESULT deleteSession(IMFTransform* pTransform);
	BOOL    hasSession(IMFTransform* pTransform);
}