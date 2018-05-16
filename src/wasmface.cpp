// source ./emsdk_env.sh --build=Release

// emcc wasmface.cpp cascade-classifier.cpp haar-like.cpp integral-image.cpp strong-classifier.cpp utility.cpp weak-classifier.cpp -s TOTAL_MEMORY=1024MB -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall', 'cwrap']" -s WASM=1 -O3 -std=c++1z -o ../demo/wasmface.js

#include <iostream>
#include <vector>
#include <emscripten/emscripten.h>

#include "../lib/json.hpp"

#include "wasmface.h"
#include "utility.h"
#include "integral-image.h"
#include "strong-classifier.h"
#include "cascade-classifier.h"


#ifdef __cplusplus
extern "C" {
#endif

EMSCRIPTEN_KEEPALIVE bool isFace(unsigned char inputBuf[]) {
	// we need to take the buffer, convert it to grayscale, convert it to an integral image, 
	// call CascadeClassifier::classify() on it and return the bool
	auto fpgs = toGrayscaleFloat(inputBuf, 240, 240);
	auto integral = IntegralImage(fpgs, 240, 240, 230400, false);
	auto integralSquared = IntegralImage(fpgs, 240, 240, 230400, true);
	delete [] fpgs;
		
	float mean = integral.data[239][239] / float(240 * 240);
	float sd = std::sqrt(integralSquared.data[239][239] / float(240 * 240) - (mean * mean));

	// For testing purposes, let's just construct a cascade classifier object from hardcoded serialized JSON
	//auto ccJSON = nlohmann::json::parse("{\"baseResolution\":24,\"fnr\":0.00995767954736948,\"fpr\":0.0003036437265109271,\"strongClassifiers\":[{\"threshold\":-3.0059447288513184,\"weakClassifiers\":[{\"h\":2,\"polarity\":1,\"threshold\":-326.0,\"type\":3,\"w\":12,\"x\":8,\"y\":8},{\"h\":2,\"polarity\":-1,\"threshold\":146.0,\"type\":1,\"w\":4,\"x\":15,\"y\":4},{\"h\":3,\"polarity\":-1,\"threshold\":24.0,\"type\":1,\"w\":1,\"x\":14,\"y\":7}],\"weights\":[2.778935194015503,1.9032222032546997,1.6762127876281738]},{\"threshold\":-3.120239734649658,\"weakClassifiers\":[{\"h\":2,\"polarity\":-1,\"threshold\":304.0,\"type\":3,\"w\":8,\"x\":10,\"y\":5},{\"h\":4,\"polarity\":1,\"threshold\":-169.0,\"type\":5,\"w\":3,\"x\":9,\"y\":7},{\"h\":1,\"polarity\":1,\"threshold\":-92.0,\"type\":3,\"w\":12,\"x\":10,\"y\":9},{\"h\":2,\"polarity\":1,\"threshold\":-61.0,\"type\":5,\"w\":3,\"x\":15,\"y\":16},{\"h\":2,\"polarity\":1,\"threshold\":-124.0,\"type\":1,\"w\":4,\"x\":5,\"y\":4}],\"weights\":[2.4184412956237793,1.7975597381591797,1.6592739820480347,1.5256601572036743,1.2293753623962402]},{\"threshold\":-2.2681517601013184,\"weakClassifiers\":[{\"h\":3,\"polarity\":1,\"threshold\":-93.0,\"type\":1,\"w\":2,\"x\":11,\"y\":7},{\"h\":2,\"polarity\":-1,\"threshold\":54.0,\"type\":5,\"w\":2,\"x\":9,\"y\":16},{\"h\":3,\"polarity\":-1,\"threshold\":217.0,\"type\":5,\"w\":4,\"x\":15,\"y\":4},{\"h\":3,\"polarity\":1,\"threshold\":-86.0,\"type\":3,\"w\":3,\"x\":17,\"y\":8},{\"h\":16,\"polarity\":-1,\"threshold\":849.0,\"type\":1,\"w\":3,\"x\":18,\"y\":2}],\"weights\":[1.8505480289459229,1.6432290077209473,1.4769548177719116,1.3432700634002686,1.3593100309371948]},{\"threshold\":-3.2237143516540527,\"weakClassifiers\":[{\"h\":2,\"polarity\":-1,\"threshold\":140.0,\"type\":3,\"w\":2,\"x\":11,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-54.0,\"type\":3,\"w\":3,\"x\":8,\"y\":9},{\"h\":1,\"polarity\":-1,\"threshold\":14.0,\"type\":1,\"w\":1,\"x\":14,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-100.0,\"type\":3,\"w\":9,\"x\":10,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-137.0,\"type\":3,\"w\":3,\"x\":17,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-1220.0,\"type\":2,\"w\":7,\"x\":3,\"y\":20},{\"h\":2,\"polarity\":-1,\"threshold\":51.0,\"type\":5,\"w\":3,\"x\":6,\"y\":9},{\"h\":2,\"polarity\":-1,\"threshold\":137.0,\"type\":3,\"w\":8,\"x\":11,\"y\":19},{\"h\":6,\"polarity\":1,\"threshold\":-75.0,\"type\":1,\"w\":2,\"x\":11,\"y\":4},{\"h\":1,\"polarity\":-1,\"threshold\":105.0,\"type\":3,\"w\":6,\"x\":11,\"y\":15},{\"h\":2,\"polarity\":-1,\"threshold\":30.0,\"type\":3,\"w\":6,\"x\":12,\"y\":6},{\"h\":1,\"polarity\":-1,\"threshold\":26.0,\"type\":3,\"w\":1,\"x\":14,\"y\":13},{\"h\":2,\"polarity\":1,\"threshold\":-4951.0,\"type\":4,\"w\":20,\"x\":4,\"y\":9},{\"h\":1,\"polarity\":1,\"threshold\":-36.0,\"type\":5,\"w\":2,\"x\":10,\"y\":16}],\"weights\":[1.3360955715179443,1.1776816844940186,1.1474370956420898,1.1071972846984863,1.0809006690979004,1.0260658264160156,0.9168733358383179,0.8652610182762146,0.808979332447052,0.8148959875106812,0.8940879702568054,0.795691192150116,0.8075473308563232,0.7358518838882446]},{\"threshold\":-2.256218194961548,\"weakClassifiers\":[{\"h\":2,\"polarity\":1,\"threshold\":-533.0,\"type\":5,\"w\":9,\"x\":0,\"y\":5},{\"h\":4,\"polarity\":-1,\"threshold\":111.0,\"type\":1,\"w\":2,\"x\":14,\"y\":6},{\"h\":2,\"polarity\":1,\"threshold\":-90.0,\"type\":5,\"w\":2,\"x\":16,\"y\":16},{\"h\":1,\"polarity\":1,\"threshold\":-87.0,\"type\":3,\"w\":5,\"x\":8,\"y\":9},{\"h\":1,\"polarity\":1,\"threshold\":-25.0,\"type\":3,\"w\":3,\"x\":13,\"y\":14},{\"h\":3,\"polarity\":-1,\"threshold\":50.0,\"type\":3,\"w\":3,\"x\":17,\"y\":11},{\"h\":24,\"polarity\":1,\"threshold\":-9821.0,\"type\":1,\"w\":9,\"x\":0,\"y\":0},{\"h\":4,\"polarity\":1,\"threshold\":-48.0,\"type\":5,\"w\":1,\"x\":14,\"y\":3},{\"h\":3,\"polarity\":-1,\"threshold\":58.0,\"type\":3,\"w\":4,\"x\":10,\"y\":5},{\"h\":2,\"polarity\":1,\"threshold\":-245.0,\"type\":5,\"w\":5,\"x\":7,\"y\":20},{\"h\":3,\"polarity\":1,\"threshold\":-70.0,\"type\":3,\"w\":2,\"x\":9,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-330.0,\"type\":1,\"w\":8,\"x\":7,\"y\":23},{\"h\":2,\"polarity\":1,\"threshold\":-1243.0,\"type\":4,\"w\":5,\"x\":15,\"y\":8},{\"h\":2,\"polarity\":-1,\"threshold\":343.0,\"type\":1,\"w\":6,\"x\":12,\"y\":1},{\"h\":2,\"polarity\":1,\"threshold\":-77.0,\"type\":3,\"w\":3,\"x\":18,\"y\":8},{\"h\":1,\"polarity\":-1,\"threshold\":112.0,\"type\":3,\"w\":6,\"x\":12,\"y\":15},{\"h\":4,\"polarity\":-1,\"threshold\":-241.0,\"type\":5,\"w\":5,\"x\":10,\"y\":9},{\"h\":4,\"polarity\":1,\"threshold\":-67.0,\"type\":1,\"w\":1,\"x\":13,\"y\":10},{\"h\":3,\"polarity\":1,\"threshold\":-143.0,\"type\":3,\"w\":5,\"x\":7,\"y\":7}],\"weights\":[1.005578637123108,1.0108798742294312,1.00043523311615,0.8881425261497498,0.8965861797332764,0.773398220539093,0.855128288269043,0.7975814342498779,0.7476156949996948,0.749822199344635,0.6943609714508057,0.743733823299408,0.7268977165222168,0.6926982998847961,0.6343910098075867,0.6697241067886353,0.6172419786453247,0.6682276129722595,0.5619039535522461]}]}");
	//auto ccJSON = nlohmann::json::parse("{\"baseResolution\":24,\"fnr\":0.02497575245797634,\"fpr\":0.0004048582923132926,\"strongClassifiers\":[{\"threshold\":-6.30743408203125,\"weakClassifiers\":[{\"h\":2,\"polarity\":1,\"threshold\":-346.0,\"type\":3,\"w\":13,\"x\":8,\"y\":8},{\"h\":4,\"polarity\":1,\"threshold\":-563.0,\"type\":1,\"w\":6,\"x\":4,\"y\":3},{\"h\":4,\"polarity\":-1,\"threshold\":52.0,\"type\":1,\"w\":2,\"x\":14,\"y\":6}],\"weights\":[2.7997231483459473,1.8736002445220947,1.634110689163208]},{\"threshold\":-2.111915349960327,\"weakClassifiers\":[{\"h\":2,\"polarity\":1,\"threshold\":-346.0,\"type\":3,\"w\":13,\"x\":8,\"y\":8},{\"h\":4,\"polarity\":1,\"threshold\":-563.0,\"type\":1,\"w\":6,\"x\":4,\"y\":3},{\"h\":4,\"polarity\":-1,\"threshold\":52.0,\"type\":1,\"w\":2,\"x\":14,\"y\":6},{\"h\":3,\"polarity\":-1,\"threshold\":64.0,\"type\":3,\"w\":2,\"x\":15,\"y\":5}],\"weights\":[2.7997231483459473,1.8736002445220947,1.634110689163208,1.4039275646209717]},{\"threshold\":-3.230884313583374,\"weakClassifiers\":[{\"h\":3,\"polarity\":1,\"threshold\":-84.0,\"type\":1,\"w\":2,\"x\":11,\"y\":7},{\"h\":2,\"polarity\":-1,\"threshold\":221.0,\"type\":3,\"w\":7,\"x\":11,\"y\":5},{\"h\":2,\"polarity\":1,\"threshold\":-40.0,\"type\":5,\"w\":2,\"x\":16,\"y\":16},{\"h\":1,\"polarity\":-1,\"threshold\":39.0,\"type\":1,\"w\":3,\"x\":17,\"y\":12},{\"h\":1,\"polarity\":1,\"threshold\":-103.0,\"type\":3,\"w\":18,\"x\":4,\"y\":9}],\"weights\":[2.2889084815979004,1.8611282110214233,1.7159672975540161,1.311781406402588,1.323338270187378]},{\"threshold\":-3.2944889068603516,\"weakClassifiers\":[{\"h\":3,\"polarity\":-1,\"threshold\":40.0,\"type\":1,\"w\":1,\"x\":14,\"y\":7},{\"h\":3,\"polarity\":1,\"threshold\":-203.0,\"type\":5,\"w\":4,\"x\":5,\"y\":4},{\"h\":2,\"polarity\":-1,\"threshold\":47.0,\"type\":5,\"w\":2,\"x\":9,\"y\":16},{\"h\":1,\"polarity\":-1,\"threshold\":73.0,\"type\":1,\"w\":4,\"x\":16,\"y\":5},{\"h\":3,\"polarity\":1,\"threshold\":-64.0,\"type\":3,\"w\":2,\"x\":17,\"y\":8},{\"h\":2,\"polarity\":-1,\"threshold\":121.0,\"type\":5,\"w\":4,\"x\":14,\"y\":20}],\"weights\":[2.179246664047241,1.6919456720352173,1.5271271467208862,1.2519487142562866,1.4140760898590088,1.1179333925247192]},{\"threshold\":-3.4525961875915527,\"weakClassifiers\":[{\"h\":1,\"polarity\":-1,\"threshold\":202.0,\"type\":3,\"w\":9,\"x\":9,\"y\":6},{\"h\":4,\"polarity\":1,\"threshold\":-170.0,\"type\":5,\"w\":3,\"x\":9,\"y\":7},{\"h\":2,\"polarity\":1,\"threshold\":-210.0,\"type\":3,\"w\":4,\"x\":16,\"y\":8},{\"h\":18,\"polarity\":1,\"threshold\":-1742.0,\"type\":1,\"w\":4,\"x\":3,\"y\":2},{\"h\":4,\"polarity\":1,\"threshold\":-9295.0,\"type\":4,\"w\":18,\"x\":6,\"y\":6},{\"h\":2,\"polarity\":-1,\"threshold\":92.0,\"type\":5,\"w\":4,\"x\":8,\"y\":17},{\"h\":6,\"polarity\":-1,\"threshold\":67.0,\"type\":5,\"w\":1,\"x\":14,\"y\":8},{\"h\":2,\"polarity\":1,\"threshold\":-208.0,\"type\":3,\"w\":12,\"x\":8,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-77.0,\"type\":3,\"w\":4,\"x\":10,\"y\":9}],\"weights\":[1.3036869764328003,1.2398349046707153,1.1294440031051636,1.0947657823562622,1.0101873874664307,1.0197765827178955,0.9403834939002991,0.9004265666007996,0.8942066431045532]},{\"threshold\":-3.8216793537139893,\"weakClassifiers\":[{\"h\":2,\"polarity\":-1,\"threshold\":692.0,\"type\":3,\"w\":11,\"x\":9,\"y\":5},{\"h\":2,\"polarity\":-1,\"threshold\":170.0,\"type\":3,\"w\":7,\"x\":11,\"y\":19},{\"h\":2,\"polarity\":1,\"threshold\":-51.0,\"type\":1,\"w\":2,\"x\":11,\"y\":7},{\"h\":18,\"polarity\":-1,\"threshold\":1130.0,\"type\":1,\"w\":3,\"x\":18,\"y\":1},{\"h\":1,\"polarity\":1,\"threshold\":-46.0,\"type\":3,\"w\":3,\"x\":8,\"y\":9},{\"h\":2,\"polarity\":-1,\"threshold\":52.0,\"type\":3,\"w\":6,\"x\":14,\"y\":11},{\"h\":1,\"polarity\":1,\"threshold\":-194.0,\"type\":2,\"w\":1,\"x\":0,\"y\":23},{\"h\":4,\"polarity\":-1,\"threshold\":180.0,\"type\":5,\"w\":5,\"x\":13,\"y\":3},{\"h\":3,\"polarity\":1,\"threshold\":-61.0,\"type\":3,\"w\":2,\"x\":18,\"y\":8},{\"h\":1,\"polarity\":-1,\"threshold\":130.0,\"type\":3,\"w\":6,\"x\":11,\"y\":15},{\"h\":3,\"polarity\":-1,\"threshold\":32.0,\"type\":1,\"w\":1,\"x\":14,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-60.0,\"type\":3,\"w\":8,\"x\":11,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-4704.0,\"type\":4,\"w\":18,\"x\":6,\"y\":8},{\"h\":1,\"polarity\":-1,\"threshold\":78.0,\"type\":3,\"w\":4,\"x\":12,\"y\":16},{\"h\":3,\"polarity\":1,\"threshold\":-74.0,\"type\":3,\"w\":2,\"x\":9,\"y\":8},{\"h\":17,\"polarity\":1,\"threshold\":-2606.0,\"type\":1,\"w\":5,\"x\":1,\"y\":3},{\"h\":3,\"polarity\":-1,\"threshold\":49.0,\"type\":3,\"w\":3,\"x\":11,\"y\":5},{\"h\":3,\"polarity\":-1,\"threshold\":17.0,\"type\":3,\"w\":4,\"x\":9,\"y\":11},{\"h\":6,\"polarity\":-1,\"threshold\":1006.0,\"type\":1,\"w\":5,\"x\":0,\"y\":0},{\"h\":3,\"polarity\":-1,\"threshold\":57.0,\"type\":1,\"w\":3,\"x\":13,\"y\":7},{\"h\":3,\"polarity\":1,\"threshold\":-61.0,\"type\":5,\"w\":2,\"x\":16,\"y\":16},{\"h\":1,\"polarity\":1,\"threshold\":-55.0,\"type\":3,\"w\":8,\"x\":12,\"y\":9},{\"h\":5,\"polarity\":1,\"threshold\":-952.0,\"type\":2,\"w\":1,\"x\":1,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":67.0,\"type\":1,\"w\":2,\"x\":13,\"y\":4},{\"h\":1,\"polarity\":-1,\"threshold\":69.0,\"type\":3,\"w\":2,\"x\":17,\"y\":7},{\"h\":4,\"polarity\":-1,\"threshold\":-345.0,\"type\":5,\"w\":6,\"x\":8,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-9.0,\"type\":5,\"w\":1,\"x\":15,\"y\":14},{\"h\":1,\"polarity\":1,\"threshold\":-34.0,\"type\":3,\"w\":2,\"x\":15,\"y\":17}],\"weights\":[1.1518512964248657,1.0733481645584106,0.9987753033638,1.0613383054733276,0.9908053874969482,0.9187448620796204,0.9908851385116577,0.9303500652313232,0.8096530437469482,0.9741901159286499,0.7836743593215942,0.6965053677558899,0.7313429117202759,0.7275989055633545,0.8575445413589478,0.7182418704032898,0.833795964717865,0.6953150629997253,0.7818726301193237,0.7098212838172913,0.6089093089103699,0.6112208962440491,0.7206677198410034,0.5988712310791016,0.6167942881584167,0.5978108644485474,0.6424809694290161,0.5525639057159424]}]}");
	auto ccJSON = nlohmann::json::parse("{\"baseResolution\":24,\"fnr\":0.0387972854077816,\"fpr\":0.0003036437265109271,\"strongClassifiers\":[{\"threshold\":-2.5062551498413086,\"weakClassifiers\":[{\"h\":3,\"polarity\":-1,\"threshold\":24.789419174194336,\"type\":4,\"w\":10,\"x\":9,\"y\":4},{\"h\":2,\"polarity\":-1,\"threshold\":-8.332386016845703,\"type\":5,\"w\":1,\"x\":1,\"y\":2},{\"h\":11,\"polarity\":1,\"threshold\":-35.86311721801758,\"type\":5,\"w\":6,\"x\":3,\"y\":2}],\"weights\":[2.334871292114258,2.0305004119873047,2.2018842697143555]},{\"threshold\":-1.6985896825790405,\"weakClassifiers\":[{\"h\":3,\"polarity\":-1,\"threshold\":24.789419174194336,\"type\":4,\"w\":10,\"x\":9,\"y\":4},{\"h\":3,\"polarity\":-1,\"threshold\":0.4747810363769531,\"type\":1,\"w\":1,\"x\":14,\"y\":7},{\"h\":3,\"polarity\":1,\"threshold\":-1.7435455322265625,\"type\":1,\"w\":3,\"x\":10,\"y\":7},{\"h\":3,\"polarity\":1,\"threshold\":-1.6587066650390625,\"type\":3,\"w\":3,\"x\":17,\"y\":8}],\"weights\":[2.3166720867156982,1.6900537014007568,1.2557719945907593,1.0694360733032227]},{\"threshold\":-3.541293144226074,\"weakClassifiers\":[{\"h\":3,\"polarity\":-1,\"threshold\":34.62174606323242,\"type\":4,\"w\":12,\"x\":8,\"y\":4},{\"h\":1,\"polarity\":1,\"threshold\":-2.3591766357421875,\"type\":3,\"w\":14,\"x\":7,\"y\":9},{\"h\":1,\"polarity\":-1,\"threshold\":0.3657684326171875,\"type\":1,\"w\":1,\"x\":14,\"y\":8},{\"h\":5,\"polarity\":1,\"threshold\":-3.4917831420898438,\"type\":5,\"w\":4,\"x\":6,\"y\":2},{\"h\":1,\"polarity\":-1,\"threshold\":1.5315933227539063,\"type\":3,\"w\":7,\"x\":11,\"y\":20},{\"h\":6,\"polarity\":-1,\"threshold\":4.1089324951171875,\"type\":5,\"w\":3,\"x\":15,\"y\":2},{\"h\":4,\"polarity\":1,\"threshold\":-1.3072509765625,\"type\":5,\"w\":3,\"x\":9,\"y\":7},{\"h\":21,\"polarity\":-1,\"threshold\":83.9234619140625,\"type\":1,\"w\":6,\"x\":0,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-1.23992919921875,\"type\":3,\"w\":8,\"x\":8,\"y\":9}],\"weights\":[1.8683208227157593,1.238745093345642,1.2128159999847412,0.9837888479232788,0.9851679801940918,0.9067387580871582,0.8079342842102051,0.871544361114502,0.8756822347640991]},{\"threshold\":-3.067190170288086,\"weakClassifiers\":[{\"h\":1,\"polarity\":-1,\"threshold\":4.5278778076171875,\"type\":4,\"w\":8,\"x\":10,\"y\":3},{\"h\":2,\"polarity\":-1,\"threshold\":0.9369659423828125,\"type\":5,\"w\":2,\"x\":9,\"y\":16},{\"h\":3,\"polarity\":1,\"threshold\":-2.3243408203125,\"type\":1,\"w\":2,\"x\":11,\"y\":7},{\"h\":1,\"polarity\":1,\"threshold\":-3.9069042205810547,\"type\":3,\"w\":12,\"x\":8,\"y\":9},{\"h\":1,\"polarity\":-1,\"threshold\":1.403228759765625,\"type\":3,\"w\":5,\"x\":12,\"y\":21},{\"h\":2,\"polarity\":-1,\"threshold\":3.435958862304006875,\"type\":3,\"w\":6,\"x\":12,\"y\":5},{\"h\":5,\"polarity\":-1,\"threshold\":1.2179546356201172,\"type\":1,\"w\":1,\"x\":14,\"y\":7},{\"h\":19,\"polarity\":-1,\"threshold\":19.8267822265625,\"type\":1,\"w\":3,\"x\":18,\"y\":1},{\"h\":3,\"polarity\":1,\"threshold\":-1.66229248046875,\"type\":3,\"w\":3,\"x\":17,\"y\":8},{\"h\":19,\"polarity\":-1,\"threshold\":85.33638763427734,\"type\":1,\"w\":6,\"x\":0,\"y\":0},{\"h\":3,\"polarity\":-1,\"threshold\":0.01277923583984375,\"type\":3,\"w\":3,\"x\":9,\"y\":11},{\"h\":21,\"polarity\":1,\"threshold\":-24.39348602294922,\"type\":1,\"w\":3,\"x\":4,\"y\":3},{\"h\":3,\"polarity\":1,\"threshold\":-1.9667034149169922,\"type\":5,\"w\":5,\"x\":4,\"y\":5},{\"h\":2,\"polarity\":1,\"threshold\":-0.01177978515625,\"type\":3,\"w\":1,\"x\":15,\"y\":17},{\"h\":1,\"polarity\":-1,\"threshold\":0.36467742919921875,\"type\":5,\"w\":1,\"x\":10,\"y\":8},{\"h\":2,\"polarity\":-1,\"threshold\":0.0507049560546875,\"type\":3,\"w\":8,\"x\":13,\"y\":11}],\"weights\":[1.207371711730957,1.1398316621780396,1.085977554321289,1.0559720993041992,0.8888664841651917,0.8403931856155396,0.8520782589912415,0.8626779317855835,0.8453302383422852,0.9320998787879944,0.8462998270988464,0.8054268956184387,0.7752200961112976,0.7247264385223389,0.7002906203269958,0.7173691987991333]},{\"threshold\":-3.2289841175079346,\"weakClassifiers\":[{\"h\":2,\"polarity\":1,\"threshold\":-1.2386322021484375,\"type\":5,\"w\":2,\"x\":16,\"y\":16},{\"h\":3,\"polarity\":1,\"threshold\":-0.9442462921142578,\"type\":5,\"w\":1,\"x\":14,\"y\":4},{\"h\":3,\"polarity\":-1,\"threshold\":39.463531494140625,\"type\":4,\"w\":10,\"x\":9,\"y\":4},{\"h\":4,\"polarity\":-1,\"threshold\":1.5952911376953125,\"type\":1,\"w\":2,\"x\":14,\"y\":6},{\"h\":13,\"polarity\":1,\"threshold\":-33.69037628173828,\"type\":1,\"w\":4,\"x\":3,\"y\":1},{\"h\":3,\"polarity\":1,\"threshold\":-3.313976287841797,\"type\":5,\"w\":4,\"x\":6,\"y\":4},{\"h\":1,\"polarity\":-1,\"threshold\":1.0323486328125,\"type\":3,\"w\":6,\"x\":10,\"y\":20},{\"h\":1,\"polarity\":1,\"threshold\":-0.20378875732421875,\"type\":5,\"w\":1,\"x\":15,\"y\":14},{\"h\":3,\"polarity\":1,\"threshold\":-2.3375606536865234,\"type\":3,\"w\":3,\"x\":9,\"y\":7},{\"h\":1,\"polarity\":-1,\"threshold\":0.3952369689941406,\"type\":5,\"w\":1,\"x\":14,\"y\":13},{\"h\":4,\"polarity\":-1,\"threshold\":0.21608734130859375,\"type\":3,\"w\":11,\"x\":8,\"y\":4},{\"h\":8,\"polarity\":-1,\"threshold\":5.673552989959717,\"type\":1,\"w\":2,\"x\":20,\"y\":4},{\"h\":4,\"polarity\":1,\"threshold\":-0.013271331787109375,\"type\":1,\"w\":4,\"x\":9,\"y\":7},{\"h\":1,\"polarity\":-1,\"threshold\":0.6324462890625,\"type\":5,\"w\":1,\"x\":10,\"y\":8},{\"h\":5,\"polarity\":1,\"threshold\":5.9173126220703125,\"type\":3,\"w\":11,\"x\":10,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-0.48947906494140625,\"type\":5,\"w\":1,\"x\":11,\"y\":16},{\"h\":2,\"polarity\":-1,\"threshold\":0.0135040283203125,\"type\":1,\"w\":2,\"x\":9,\"y\":12},{\"h\":1,\"polarity\":-1,\"threshold\":0.214111328125,\"type\":5,\"w\":1,\"x\":15,\"y\":13},{\"h\":2,\"polarity\":-1,\"threshold\":6.103515625e-05,\"type\":3,\"w\":3,\"x\":17,\"y\":12},{\"h\":1,\"polarity\":-1,\"threshold\":0.7151880264282227,\"type\":3,\"w\":2,\"x\":12,\"y\":15},{\"h\":3,\"polarity\":-1,\"threshold\":-4.1066436767578125,\"type\":5,\"w\":6,\"x\":9,\"y\":10},{\"h\":18,\"polarity\":-1,\"threshold\":65.59468078613281,\"type\":2,\"w\":5,\"x\":0,\"y\":4},{\"h\":2,\"polarity\":1,\"threshold\":2.636402130126953,\"type\":5,\"w\":6,\"x\":8,\"y\":11},{\"h\":1,\"polarity\":1,\"threshold\":-1.0292587280273438,\"type\":2,\"w\":1,\"x\":2,\"y\":23},{\"h\":3,\"polarity\":-1,\"threshold\":0.9931793212890625,\"type\":5,\"w\":4,\"x\":15,\"y\":5},{\"h\":2,\"polarity\":-1,\"threshold\":10.485763549804688,\"type\":1,\"w\":7,\"x\":1,\"y\":22},{\"h\":2,\"polarity\":-1,\"threshold\":-0.3022308349609375,\"type\":3,\"w\":1,\"x\":7,\"y\":10},{\"h\":1,\"polarity\":1,\"threshold\":-0.011962890625,\"type\":5,\"w\":1,\"x\":17,\"y\":19},{\"h\":1,\"polarity\":1,\"threshold\":-0.012583136558532715,\"type\":3,\"w\":1,\"x\":15,\"y\":0},{\"h\":19,\"polarity\":1,\"threshold\":-9.838930130004883,\"type\":2,\"w\":1,\"x\":2,\"y\":2},{\"h\":1,\"polarity\":1,\"threshold\":-2.6847000122070313,\"type\":3,\"w\":20,\"x\":2,\"y\":9},{\"h\":1,\"polarity\":-1,\"threshold\":0.318511962890625,\"type\":5,\"w\":1,\"x\":12,\"y\":15},{\"h\":6,\"polarity\":1,\"threshold\":23.83830261230400688,\"type\":3,\"w\":9,\"x\":10,\"y\":8},{\"h\":18,\"polarity\":-1,\"threshold\":60.329803466796875,\"type\":1,\"w\":5,\"x\":1,\"y\":0},{\"h\":6,\"polarity\":-1,\"threshold\":-0.5069732666015625,\"type\":3,\"w\":1,\"x\":14,\"y\":6},{\"h\":24,\"polarity\":-1,\"threshold\":9.467742919921875,\"type\":2,\"w\":1,\"x\":12,\"y\":0},{\"h\":2,\"polarity\":-1,\"threshold\":-0.807891845703125,\"type\":1,\"w\":2,\"x\":12,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-0.4893646240234375,\"type\":5,\"w\":1,\"x\":17,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":0.5551834106445313,\"type\":1,\"w\":2,\"x\":12,\"y\":4},{\"h\":1,\"polarity\":1,\"threshold\":-0.45218992233276367,\"type\":5,\"w\":1,\"x\":10,\"y\":9}],\"weights\":[1.085605502128601,0.9965683221817017,1.0058809518814087,0.867337703704834,0.8645536303520203,0.7673463225364685,0.7823313474655151,0.7286639213562012,0.7887197136878967,0.8129231333732605,0.7755913138389587,0.7411670088768005,0.6603976488113403,0.687011182308197,0.737315833568573,0.6802538633346558,0.6465259790420532,0.6096183061599731,0.6762983202934265,0.6496338844299316,0.5678293108940125,0.6630011200904846,0.7197701334953308,0.6033223867416382,0.6183159947395325,0.5836629271507263,0.6102038621902466,0.5307769775390625,0.5172045826911926,0.5131242871284485,0.6697249412536621,0.5276651382446289,0.5940514802932739,0.5601378679275513,0.6723397374153137,0.5146013498306274,0.5835533738136292,0.5596929788589478,0.6503955125808716,0.5435855388641357]},{\"threshold\":-3.089313507080078,\"weakClassifiers\":[{\"h\":3,\"polarity\":-1,\"threshold\":46.64955139160156,\"type\":4,\"w\":11,\"x\":9,\"y\":4},{\"h\":6,\"polarity\":-1,\"threshold\":1.7695465087890625,\"type\":1,\"w\":2,\"x\":14,\"y\":4},{\"h\":19,\"polarity\":-1,\"threshold\":31.040002822875977,\"type\":1,\"w\":3,\"x\":18,\"y\":1},{\"h\":1,\"polarity\":1,\"threshold\":-0.0113983154296875,\"type\":1,\"w\":1,\"x\":16,\"y\":12},{\"h\":2,\"polarity\":1,\"threshold\":-0.4365692138671875,\"type\":5,\"w\":1,\"x\":14,\"y\":6},{\"h\":1,\"polarity\":-1,\"threshold\":2.9806060791015625,\"type\":3,\"w\":7,\"x\":11,\"y\":15},{\"h\":3,\"polarity\":1,\"threshold\":-1.5984573364257813,\"type\":3,\"w\":2,\"x\":9,\"y\":8},{\"h\":6,\"polarity\":1,\"threshold\":-35.52888107299805,\"type\":1,\"w\":6,\"x\":2,\"y\":1},{\"h\":3,\"polarity\":-1,\"threshold\":1.603795051574707,\"type\":5,\"w\":3,\"x\":16,\"y\":3},{\"h\":18,\"polarity\":-1,\"threshold\":41.140865325927734,\"type\":1,\"w\":5,\"x\":0,\"y\":0},{\"h\":1,\"polarity\":1,\"threshold\":0.5574874877929688,\"type\":3,\"w\":5,\"x\":15,\"y\":4},{\"h\":2,\"polarity\":-1,\"threshold\":4.443349838256836,\"type\":3,\"w\":4,\"x\":13,\"y\":20},{\"h\":2,\"polarity\":-1,\"threshold\":0.3353118896484375,\"type\":3,\"w\":10,\"x\":7,\"y\":11},{\"h\":10,\"polarity\":1,\"threshold\":-62.77935791015625,\"type\":1,\"w\":8,\"x\":0,\"y\":14},{\"h\":2,\"polarity\":-1,\"threshold\":0.0124664306640625,\"type\":3,\"w\":2,\"x\":12,\"y\":6},{\"h\":1,\"polarity\":-1,\"threshold\":1.610626220703125,\"type\":3,\"w\":3,\"x\":13,\"y\":16},{\"h\":2,\"polarity\":1,\"threshold\":-1.1530323028564453,\"type\":3,\"w\":3,\"x\":18,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-2.72528076171875,\"type\":3,\"w\":7,\"x\":11,\"y\":16},{\"h\":5,\"polarity\":1,\"threshold\":-1.1733856201171875,\"type\":1,\"w\":2,\"x\":11,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-0.43202972412109375,\"type\":5,\"w\":1,\"x\":8,\"y\":8},{\"h\":3,\"polarity\":-1,\"threshold\":-0.6476707458496094,\"type\":1,\"w\":2,\"x\":9,\"y\":10},{\"h\":17,\"polarity\":1,\"threshold\":-62.726654052734375,\"type\":1,\"w\":6,\"x\":0,\"y\":2},{\"h\":2,\"polarity\":1,\"threshold\":-0.0151519775390625,\"type\":3,\"w\":5,\"x\":13,\"y\":9},{\"h\":23,\"polarity\":-1,\"threshold\":74.4865112304006875,\"type\":2,\"w\":8,\"x\":0,\"y\":0},{\"h\":1,\"polarity\":-1,\"threshold\":-0.5964546203613281,\"type\":1,\"w\":2,\"x\":12,\"y\":5},{\"h\":2,\"polarity\":1,\"threshold\":-4.596770286560059,\"type\":3,\"w\":9,\"x\":11,\"y\":0},{\"h\":1,\"polarity\":1,\"threshold\":-1.0570068359375,\"type\":5,\"w\":2,\"x\":11,\"y\":13},{\"h\":3,\"polarity\":1,\"threshold\":4.201993942260742,\"type\":5,\"w\":6,\"x\":8,\"y\":9}],\"weights\":[1.0063867568969727,0.7685754895210266,0.8142946362495422,0.7166441679000854,0.650409460067749,0.6561197638511658,0.6765909194946289,0.74831223487854,0.6034961342811584,0.6263085603713989,0.6780802011489868,0.609100341796875,0.6031295657157898,0.6025173664093018,0.6118225455284119,0.5936284065246582,0.5684784054756165,0.5717772245407104,0.5823402404785156,0.5486648082733154,0.5397675633430481,0.5546543002128601,0.5586724877357483,0.6118795871734619,0.5809723734855652,0.5602096319198608,0.48816707730293274,0.5201800465583801]},{\"threshold\":-3.4528465270996094,\"weakClassifiers\":[{\"h\":6,\"polarity\":-1,\"threshold\":19.01477813720703,\"type\":1,\"w\":4,\"x\":16,\"y\":1},{\"h\":3,\"polarity\":-1,\"threshold\":0.9232063293457031,\"type\":1,\"w\":1,\"x\":14,\"y\":7},{\"h\":20,\"polarity\":1,\"threshold\":-47.11388397216797,\"type\":1,\"w\":4,\"x\":4,\"y\":1},{\"h\":1,\"polarity\":-1,\"threshold\":3.035633087158203,\"type\":3,\"w\":7,\"x\":10,\"y\":6},{\"h\":2,\"polarity\":-1,\"threshold\":1.259124755859375,\"type\":5,\"w\":2,\"x\":9,\"y\":16},{\"h\":1,\"polarity\":-1,\"threshold\":0.0150146484375,\"type\":3,\"w\":2,\"x\":9,\"y\":13},{\"h\":6,\"polarity\":1,\"threshold\":-2.5511932373046875,\"type\":1,\"w\":1,\"x\":13,\"y\":8},{\"h\":3,\"polarity\":1,\"threshold\":-3.87115478515625,\"type\":3,\"w\":3,\"x\":17,\"y\":7},{\"h\":15,\"polarity\":1,\"threshold\":-41.43525695800781,\"type\":1,\"w\":4,\"x\":1,\"y\":2},{\"h\":4,\"polarity\":1,\"threshold\":-0.6503372192382813,\"type\":1,\"w\":3,\"x\":9,\"y\":7},{\"h\":4,\"polarity\":-1,\"threshold\":12.74626350402832,\"type\":1,\"w\":5,\"x\":1,\"y\":0},{\"h\":1,\"polarity\":1,\"threshold\":-0.013824462890625,\"type\":3,\"w\":2,\"x\":12,\"y\":9},{\"h\":1,\"polarity\":1,\"threshold\":-6.833223342895508,\"type\":2,\"w\":6,\"x\":6,\"y\":20},{\"h\":2,\"polarity\":-1,\"threshold\":-4.472155570983887,\"type\":5,\"w\":6,\"x\":8,\"y\":10},{\"h\":8,\"polarity\":-1,\"threshold\":39.331939697265625,\"type\":1,\"w\":9,\"x\":3,\"y\":16},{\"h\":2,\"polarity\":1,\"threshold\":-0.01287841796875,\"type\":3,\"w\":2,\"x\":16,\"y\":16},{\"h\":5,\"polarity\":1,\"threshold\":-39.97331619262695,\"type\":1,\"w\":9,\"x\":0,\"y\":1},{\"h\":1,\"polarity\":1,\"threshold\":-0.012681961059570313,\"type\":3,\"w\":1,\"x\":15,\"y\":14},{\"h\":2,\"polarity\":-1,\"threshold\":1.2124576568603516,\"type\":5,\"w\":3,\"x\":6,\"y\":9},{\"h\":1,\"polarity\":-1,\"threshold\":0.8525161743164063,\"type\":5,\"w\":1,\"x\":10,\"y\":8},{\"h\":3,\"polarity\":1,\"threshold\":4.226226806640625,\"type\":5,\"w\":6,\"x\":7,\"y\":10},{\"h\":3,\"polarity\":1,\"threshold\":-3.059070587158203,\"type\":2,\"w\":1,\"x\":0,\"y\":21},{\"h\":3,\"polarity\":-1,\"threshold\":0.4074859619140625,\"type\":5,\"w\":4,\"x\":14,\"y\":3},{\"h\":5,\"polarity\":-1,\"threshold\":20.683910369873047,\"type\":1,\"w\":7,\"x\":0,\"y\":19},{\"h\":2,\"polarity\":-1,\"threshold\":-0.4995231628417969,\"type\":1,\"w\":1,\"x\":12,\"y\":4},{\"h\":1,\"polarity\":-1,\"threshold\":5.05426025390625,\"type\":4,\"w\":6,\"x\":11,\"y\":15},{\"h\":4,\"polarity\":1,\"threshold\":-0.013454437255859375,\"type\":1,\"w\":1,\"x\":12,\"y\":5},{\"h\":21,\"polarity\":1,\"threshold\":-9.130119323730469,\"type\":2,\"w\":1,\"x\":6,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-0.6999092102050781,\"type\":3,\"w\":2,\"x\":8,\"y\":8},{\"h\":19,\"polarity\":-1,\"threshold\":7.96629524230957,\"type\":2,\"w\":1,\"x\":7,\"y\":2},{\"h\":5,\"polarity\":-1,\"threshold\":-8.365020751953125,\"type\":5,\"w\":6,\"x\":9,\"y\":8},{\"h\":1,\"polarity\":1,\"threshold\":-0.439666748046875,\"type\":5,\"w\":1,\"x\":11,\"y\":16},{\"h\":4,\"polarity\":-1,\"threshold\":0.46228599548339844,\"type\":3,\"w\":5,\"x\":15,\"y\":11},{\"h\":20,\"polarity\":1,\"threshold\":-29.029754638671875,\"type\":1,\"w\":3,\"x\":18,\"y\":0},{\"h\":1,\"polarity\":1,\"threshold\":1.2690048217773438,\"type\":5,\"w\":6,\"x\":10,\"y\":11},{\"h\":11,\"polarity\":-1,\"threshold\":19.874290466308594,\"type\":1,\"w\":3,\"x\":18,\"y\":9},{\"h\":1,\"polarity\":1,\"threshold\":0.9920501708984375,\"type\":3,\"w\":3,\"x\":8,\"y\":14},{\"h\":1,\"polarity\":1,\"threshold\":-1.2802581787109375,\"type\":3,\"w\":3,\"x\":13,\"y\":13},{\"h\":3,\"polarity\":-1,\"threshold\":1.1363296508789063,\"type\":3,\"w\":5,\"x\":14,\"y\":5},{\"h\":1,\"polarity\":-1,\"threshold\":6.646430969238281,\"type\":1,\"w\":7,\"x\":0,\"y\":0},{\"h\":3,\"polarity\":-1,\"threshold\":0.0102691650390625,\"type\":3,\"w\":7,\"x\":7,\"y\":12},{\"h\":1,\"polarity\":1,\"threshold\":-0.53076171875,\"type\":1,\"w\":1,\"x\":13,\"y\":9},{\"h\":4,\"polarity\":1,\"threshold\":4.653861999511719,\"type\":3,\"w\":2,\"x\":11,\"y\":10},{\"h\":6,\"polarity\":1,\"threshold\":-5.663722991943359,\"type\":2,\"w\":1,\"x\":1,\"y\":0},{\"h\":6,\"polarity\":-1,\"threshold\":-2.7873497009277344,\"type\":1,\"w\":4,\"x\":12,\"y\":7},{\"h\":24,\"polarity\":-1,\"threshold\":41.937381744384766,\"type\":2,\"w\":7,\"x\":3,\"y\":0},{\"h\":1,\"polarity\":-1,\"threshold\":-0.43856048583984375,\"type\":3,\"w\":6,\"x\":16,\"y\":12},{\"h\":1,\"polarity\":-1,\"threshold\":0.01031494140625,\"type\":5,\"w\":1,\"x\":12,\"y\":11},{\"h\":3,\"polarity\":-1,\"threshold\":7.5561370849609375,\"type\":5,\"w\":5,\"x\":14,\"y\":18},{\"h\":1,\"polarity\":1,\"threshold\":0.4486417770385742,\"type\":5,\"w\":3,\"x\":9,\"y\":4}],\"weights\":[0.8556437492370605,0.72967928647995,0.7158645391464233,0.6931792497634888,0.650702178478241,0.6152501106262207,0.6129434704780579,0.6027109622955322,0.589823842048645,0.6473327279090881,0.5883839726448059,0.6171302795410156,0.5313611626625061,0.5252168774604797,0.5688841938972473,0.5424720644950867,0.532828688621521,0.5536643266677856,0.5226812958717346,0.5016742944717407,0.5410975217819214,0.513327956199646,0.46800893545150757,0.5334904789924622,0.517880380153656,0.5058574080467224,0.5433244109153748,0.4527408480644226,0.546646773815155,0.46278974413871765,0.5316407680511475,0.485012948513031,0.48747682571411133,0.5206363797187805,0.5455448627471924,0.43624207377433777,0.5093109607696533,0.4695828855037689,0.4991239905357361,0.43106168508529663,0.5341769456863403,0.44133904576301575,0.430219829082489,0.43517595529556274,0.45785704255104065,0.4557804763317108,0.497560054063797,0.44436776638031006,0.39093008637428284,0.4152756631374359]},{\"threshold\":-3.5697786808013916,\"weakClassifiers\":[{\"h\":10,\"polarity\":-1,\"threshold\":6.628334045410156,\"type\":2,\"w\":1,\"x\":14,\"y\":2},{\"h\":1,\"polarity\":-1,\"threshold\":1.6252899169921875,\"type\":3,\"w\":2,\"x\":16,\"y\":6},{\"h\":2,\"polarity\":-1,\"threshold\":0.9419364929199219,\"type\":5,\"w\":2,\"x\":11,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-3.122647285461426,\"type\":3,\"w\":12,\"x\":9,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-1.3727169036865234,\"type\":5,\"w\":3,\"x\":14,\"y\":17},{\"h\":1,\"polarity\":1,\"threshold\":-0.012603759765625,\"type\":5,\"w\":2,\"x\":16,\"y\":13},{\"h\":14,\"polarity\":-1,\"threshold\":43.84405517578125,\"type\":1,\"w\":4,\"x\":0,\"y\":0},{\"h\":3,\"polarity\":-1,\"threshold\":-1.550537109375,\"type\":5,\"w\":4,\"x\":11,\"y\":9},{\"h\":10,\"polarity\":1,\"threshold\":-31.171829223632813,\"type\":1,\"w\":4,\"x\":2,\"y\":2},{\"h\":2,\"polarity\":1,\"threshold\":-1.6377372741699219,\"type\":3,\"w\":4,\"x\":10,\"y\":9},{\"h\":1,\"polarity\":1,\"threshold\":-2.88128662109375,\"type\":3,\"w\":5,\"x\":12,\"y\":16},{\"h\":2,\"polarity\":1,\"threshold\":-1.2447967529296875,\"type\":5,\"w\":3,\"x\":17,\"y\":8},{\"h\":2,\"polarity\":-1,\"threshold\":0.1714935302734375,\"type\":1,\"w\":1,\"x\":11,\"y\":11},{\"h\":1,\"polarity\":1,\"threshold\":-0.8487091064453125,\"type\":5,\"w\":2,\"x\":11,\"y\":13},{\"h\":1,\"polarity\":-1,\"threshold\":1.52587890625e-05,\"type\":3,\"w\":2,\"x\":13,\"y\":6},{\"h\":1,\"polarity\":1,\"threshold\":-0.72113037109375,\"type\":5,\"w\":1,\"x\":10,\"y\":9},{\"h\":2,\"polarity\":-1,\"threshold\":0.13915252685546875,\"type\":3,\"w\":2,\"x\":11,\"y\":14},{\"h\":22,\"polarity\":1,\"threshold\":-107.3000259399414,\"type\":2,\"w\":7,\"x\":3,\"y\":0},{\"h\":4,\"polarity\":1,\"threshold\":-0.013982772827148438,\"type\":1,\"w\":2,\"x\":16,\"y\":10},{\"h\":1,\"polarity\":-1,\"threshold\":-0.7684707641601563,\"type\":1,\"w\":2,\"x\":16,\"y\":11},{\"h\":5,\"polarity\":-1,\"threshold\":4.47734260559082,\"type\":2,\"w\":1,\"x\":0,\"y\":16},{\"h\":3,\"polarity\":1,\"threshold\":4.9348907470703125,\"type\":5,\"w\":5,\"x\":10,\"y\":13},{\"h\":1,\"polarity\":-1,\"threshold\":3.5111007690429688,\"type\":4,\"w\":3,\"x\":13,\"y\":16},{\"h\":2,\"polarity\":-1,\"threshold\":0.019704818725585938,\"type\":3,\"w\":4,\"x\":15,\"y\":11},{\"h\":1,\"polarity\":-1,\"threshold\":0.5596151351928711,\"type\":5,\"w\":1,\"x\":15,\"y\":12},{\"h\":10,\"polarity\":-1,\"threshold\":1.1209144592285156,\"type\":1,\"w\":2,\"x\":14,\"y\":2},{\"h\":4,\"polarity\":1,\"threshold\":-21.521194458007813,\"type\":1,\"w\":8,\"x\":8,\"y\":20},{\"h\":1,\"polarity\":-1,\"threshold\":-0.2981839179992676,\"type\":1,\"w\":1,\"x\":12,\"y\":3},{\"h\":19,\"polarity\":-1,\"threshold\":18.298721313476563,\"type\":1,\"w\":2,\"x\":20,\"y\":0},{\"h\":2,\"polarity\":1,\"threshold\":-1.52587890625e-05,\"type\":3,\"w\":2,\"x\":12,\"y\":17},{\"h\":2,\"polarity\":1,\"threshold\":-0.012501716613769531,\"type\":5,\"w\":1,\"x\":18,\"y\":10},{\"h\":23,\"polarity\":-1,\"threshold\":110.06626892089844,\"type\":1,\"w\":10,\"x\":4,\"y\":0},{\"h\":4,\"polarity\":1,\"threshold\":-1.1939010620117188,\"type\":3,\"w\":7,\"x\":8,\"y\":7},{\"h\":5,\"polarity\":1,\"threshold\":-4.587804794311523,\"type\":2,\"w\":1,\"x\":0,\"y\":19},{\"h\":5,\"polarity\":1,\"threshold\":-4.0355987548828125,\"type\":5,\"w\":3,\"x\":7,\"y\":2},{\"h\":1,\"polarity\":-1,\"threshold\":8.09157943725586,\"type\":1,\"w\":8,\"x\":2,\"y\":23},{\"h\":1,\"polarity\":1,\"threshold\":0.49860382080078125,\"type\":1,\"w\":2,\"x\":12,\"y\":5},{\"h\":1,\"polarity\":1,\"threshold\":-0.45829010009765625,\"type\":5,\"w\":1,\"x\":13,\"y\":13},{\"h\":1,\"polarity\":1,\"threshold\":0.20900726318359375,\"type\":3,\"w\":1,\"x\":10,\"y\":4},{\"h\":1,\"polarity\":-1,\"threshold\":3.5402984619140625,\"type\":4,\"w\":4,\"x\":12,\"y\":15},{\"h\":1,\"polarity\":-1,\"threshold\":0.00984954833984375,\"type\":1,\"w\":1,\"x\":17,\"y\":4},{\"h\":2,\"polarity\":1,\"threshold\":0.8547515869140625,\"type\":5,\"w\":2,\"x\":12,\"y\":2},{\"h\":12,\"polarity\":1,\"threshold\":-23.450698852539063,\"type\":1,\"w\":3,\"x\":18,\"y\":12},{\"h\":1,\"polarity\":1,\"threshold\":1.2223052978515625,\"type\":3,\"w\":4,\"x\":18,\"y\":15},{\"h\":5,\"polarity\":1,\"threshold\":-4.464530944824219,\"type\":2,\"w\":1,\"x\":5,\"y\":17},{\"h\":1,\"polarity\":1,\"threshold\":1.07623291015625,\"type\":3,\"w\":4,\"x\":7,\"y\":15},{\"h\":1,\"polarity\":1,\"threshold\":-0.8348236083984375,\"type\":5,\"w\":1,\"x\":11,\"y\":16},{\"h\":3,\"polarity\":1,\"threshold\":-0.2284393310546875,\"type\":5,\"w\":3,\"x\":9,\"y\":9},{\"h\":4,\"polarity\":-1,\"threshold\":1.2951812744140625,\"type\":3,\"w\":3,\"x\":19,\"y\":12},{\"h\":5,\"polarity\":-1,\"threshold\":21.46139907836914,\"type\":2,\"w\":7,\"x\":3,\"y\":17},{\"h\":1,\"polarity\":-1,\"threshold\":-0.2944965362548828,\"type\":3,\"w\":2,\"x\":6,\"y\":15},{\"h\":1,\"polarity\":-1,\"threshold\":0.6474170684814453,\"type\":5,\"w\":1,\"x\":15,\"y\":13},{\"h\":1,\"polarity\":-1,\"threshold\":-1.25390625,\"type\":5,\"w\":7,\"x\":7,\"y\":4},{\"h\":1,\"polarity\":1,\"threshold\":-0.7467269897460938,\"type\":3,\"w\":3,\"x\":13,\"y\":13},{\"h\":4,\"polarity\":-1,\"threshold\":1.3437881469726563,\"type\":3,\"w\":5,\"x\":8,\"y\":10},{\"h\":1,\"polarity\":-1,\"threshold\":0.15294361114501953,\"type\":1,\"w\":2,\"x\":18,\"y\":4},{\"h\":2,\"polarity\":-1,\"threshold\":11.785012245178223,\"type\":1,\"w\":8,\"x\":0,\"y\":0},{\"h\":1,\"polarity\":1,\"threshold\":0.5291290283203125,\"type\":3,\"w\":2,\"x\":19,\"y\":14},{\"h\":9,\"polarity\":1,\"threshold\":-6.73198127746582,\"type\":2,\"w\":1,\"x\":1,\"y\":0},{\"h\":1,\"polarity\":-1,\"threshold\":-0.46942138671875,\"type\":3,\"w\":3,\"x\":19,\"y\":14}],\"weights\":[0.7256308794021606,0.6384989023208618,0.6116064190864563,0.5739752054214478,0.5593574047088623,0.5114075541496277,0.5356281399726868,0.6023207306861877,0.5389353036880493,0.6020959615707397,0.5131082534790039,0.5240722894668579,0.4672880172729492,0.44476738572120667,0.4613616466522217,0.504300594329834,0.48569992184638977,0.4342973232269287,0.5416172742843628,0.45024266839027405,0.5629423260688782,0.4972658157348633,0.4479840397834778,0.531514585018158,0.42150697112083435,0.4731786549091339,0.4351789653301239,0.4794815480709076,0.466515451669693,0.46796658635139465,0.4031147062778473,0.44439932703971863,0.49319589138031006,0.44974175095558167,0.4142637550830841,0.42242956161499023,0.5168354511260986,0.40183186531066895,0.44753655791282654,0.43977823853492737,0.4362310767173767,0.3796226382255554,0.4741586148738861,0.4781818389892578,0.41351035237312317,0.4272799491882324,0.42246994376182556,0.45016393065452576,0.37006092071533203,0.43056949973106384,0.4562646150588989,0.38251033425331116,0.42544201016426086,0.40870004892349243,0.42728039622306824,0.3665792644023895,0.43946573138237,0.41223153471946716,0.4306770861148834,0.38966888189315796]}]}");

	std::vector<StrongClassifier> sc;
	for (int i = 0; i < ccJSON["strongClassifiers"].size(); i += 1) {
		StrongClassifier strongClassifier;
		strongClassifier.threshold = ccJSON["strongClassifiers"][i]["threshold"];
		for (int j = 0; j < ccJSON["strongClassifiers"][i]["weakClassifiers"].size(); j += 1) {
			WeakClassifier weakClassifier;
			weakClassifier.haarlike.type = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["type"];
			weakClassifier.haarlike.w = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["w"];
			weakClassifier.haarlike.h = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["h"];
			weakClassifier.haarlike.x = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["x"];
			weakClassifier.haarlike.y = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["y"];
			weakClassifier.threshold = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["threshold"];
			weakClassifier.polarity = ccJSON["strongClassifiers"][i]["weakClassifiers"][j]["polarity"];
			// need to confirm that we're not reversing the order of WCs - may need to use insert()
			strongClassifier.weakClassifiers.push_back(weakClassifier);
			strongClassifier.weights.push_back(ccJSON["strongClassifiers"][i]["weights"][j]);
		}
		// need to confirm that we're not reversing the order of WCs - may need to use insert()
		sc.push_back(strongClassifier);
	}

	CascadeClassifier cc(ccJSON["baseResolution"], sc);

	cc.scale(10);

	return cc.classify(integral, 0, 0, mean, sd);
}

int main() {
	std::cout << "Made with Wasmface\n";
	return 0;
}

#ifdef __cplusplus
}
#endif