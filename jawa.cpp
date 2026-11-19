#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <vector>
#include <zlib.h>
#define STB_IMAGE_IMPLEMENTATION
#define STBI_NO_STDIO
#include "stb_image.h"
#include "jpge.h"

/* Skill sampah lu cuma modal 200+ line miskon */
typedef std::vector<uint8_t> Buf;
int main(int kismin1, char** kismin2) {
	int kismin3 = kismin1 >= 4 ? std::clamp(atoi(kismin2[3]), 1, 100) : 60;
	int kismin4 = kismin1 >= 5 ? atoi(kismin2[4]) : 16;
	FILE* kismin5 = kismin1 >= 3 ? fopen(kismin2[1], "rb") : nullptr;
	if (!kismin5) return 1;
	fseek(kismin5, 0, SEEK_END);
	long kismin6 = ftell(kismin5);
	fseek(kismin5, 0, SEEK_SET);
	Buf kismin7(kismin6);
	fread(kismin7.data(), 1, kismin6, kismin5);
	fclose(kismin5);
	Buf kismin8;
	if (!memcmp(kismin7.data(), "CWS", 3)) {
		uLongf kismin9 = (kismin7[4] | kismin7[5] << 8 | kismin7[6] << 16 | (uint32_t)kismin7[7] << 24) - 8;
		kismin8.resize(kismin9);
		z_stream kismin10{};
		kismin10.next_in = kismin7.data() + 8;
		kismin10.avail_in = kismin7.size() - 8;
		kismin10.next_out = kismin8.data();
		kismin10.avail_out = kismin9;
		inflateInit(&kismin10);
		inflate(&kismin10, Z_FINISH);
		inflateEnd(&kismin10);
	} else kismin8.assign(kismin7.begin() + 8, kismin7.end());
	size_t kismin11 = (5 + (kismin8[0] >> 3) * 4 + 7) / 8 + 4;
	Buf kismin12(kismin8.begin(), kismin8.begin() + kismin11);
	while (kismin11 + 2 <= kismin8.size()) {
		int kismin13 = (kismin8[kismin11] | kismin8[kismin11 + 1] << 8) >> 6;
		uint32_t kismin14 = kismin8[kismin11] & 63;
		bool kismin15 = kismin14 == 63;
		kismin11 += 2;
		if (kismin15) {
			kismin14 = kismin8[kismin11] | kismin8[kismin11 + 1] << 8 | kismin8[kismin11 + 2] << 16 | (uint32_t)kismin8[kismin11 + 3] << 24;
			kismin11 += 4;
		}
		Buf kismin16(kismin8.begin() + kismin11, kismin8.begin() + kismin11 + kismin14);
		kismin11 += kismin14;
		if (kismin13 == 20 || kismin13 == 36) {
			bool kismin17 = kismin13 == 36;
			uint8_t kismin18 = kismin16[2];
			int kismin19 = kismin16[3] | kismin16[4] << 8, kismin20 = kismin16[5] | kismin16[6] << 8;
			size_t kismin21 = 7 + (kismin18 == 3), kismin22 = kismin18 == 3 ? kismin16[7] + 1 : 0;
			size_t kismin23 = kismin18 == 3 ? ((kismin19 + 3) & ~3) : ((kismin19 * 2 + 3) & ~3);
			Buf kismin24(kismin18 == 5 ? (size_t)kismin19 * kismin20 * 4 : (kismin18 == 3 ? kismin22 * (3 + kismin17) + kismin23 * kismin20 : kismin23 * kismin20));
			uLongf kismin25 = kismin24.size();
			uncompress(kismin24.data(), &kismin25, &kismin16[kismin21], kismin16.size() - kismin21);
			Buf kismin26((size_t)kismin19 * kismin20 * 3), kismin27(kismin17 ? (size_t)kismin19 * kismin20 : 0);
			if (kismin18 == 3) {
				size_t kismin28 = 3 + kismin17, kismin29 = kismin22 * kismin28;
				for (int kismin30 = 0; kismin30 < kismin20; kismin30++)
					for (int kismin31 = 0; kismin31 < kismin19; kismin31++) {
						size_t kismin32 = kismin24[kismin29 + kismin30 * kismin23 + kismin31] * kismin28, kismin33 = (size_t)kismin30 * kismin19 + kismin31;
						kismin26[kismin33 * 3] = kismin24[kismin32];
						kismin26[kismin33 * 3 + 1] = kismin24[kismin32 + 1];
						kismin26[kismin33 * 3 + 2] = kismin24[kismin32 + 2];
						if (kismin17) kismin27[kismin33] = kismin24[kismin32 + 3];
					}
			} else if (kismin18 == 4) {
				for (int kismin34 = 0; kismin34 < kismin20; kismin34++)
					for (int kismin35 = 0; kismin35 < kismin19; kismin35++) {
						uint16_t kismin36 = kismin24[kismin34 * kismin23 + kismin35 * 2] << 8 | kismin24[kismin34 * kismin23 + kismin35 * 2 + 1];
						uint8_t kismin37 = (kismin36 >> 10) & 31, kismin38 = (kismin36 >> 5) & 31, kismin39 = kismin36 & 31;
						size_t kismin40 = ((size_t)kismin34 * kismin19 + kismin35) * 3;
						kismin26[kismin40] = (kismin37 << 3) | (kismin37 >> 2);
						kismin26[kismin40 + 1] = (kismin38 << 3) | (kismin38 >> 2);
						kismin26[kismin40 + 2] = (kismin39 << 3) | (kismin39 >> 2);
					}
			} else {
				for (size_t kismin41 = 0; kismin41 < (size_t)kismin19 * kismin20; kismin41++) {
					if (kismin17) kismin27[kismin41] = kismin24[kismin41 * 4];
					kismin26[kismin41 * 3] = kismin24[kismin41 * 4 + 1];
					kismin26[kismin41 * 3 + 1] = kismin24[kismin41 * 4 + 2];
					kismin26[kismin41 * 3 + 2] = kismin24[kismin41 * 4 + 3];
				}
			}
			bool kismin42 = false;
			if (kismin17) {
				if (kismin4 >= 2) for (uint8_t& kismin43 : kismin27) kismin43 = (kismin43 * (kismin4 - 1) + 127) / 255 * 255 / (kismin4 - 1);
				for (size_t kismin44 = 0; kismin44 < (size_t)kismin19 * kismin20; kismin44++) {
					if (kismin27[kismin44] == 0) kismin26[kismin44 * 3] = kismin26[kismin44 * 3 + 1] = kismin26[kismin44 * 3 + 2] = 0;
					if (kismin27[kismin44] != 255) kismin42 = true;
				}
			}
			jpge::params kismin45;
			kismin45.m_quality = kismin3;
			kismin45.m_subsampling = jpge::H2V2;
			kismin45.m_two_pass_flag = true;
			int kismin46 = std::max(4096, kismin19 * kismin20 * 3 + 65536);
			Buf kismin47(kismin46);
			jpge::compress_image_to_jpeg_file_in_memory(kismin47.data(), kismin46, kismin19, kismin20, 3, kismin26.data(), kismin45);
			kismin47.resize(kismin46);
			Buf kismin48(kismin16.begin(), kismin16.begin() + 2);
			if (kismin42) {
				Buf kismin49(compressBound(kismin27.size()));
				uLongf kismin50 = kismin49.size();
				compress2(kismin49.data(), &kismin50, kismin27.data(), kismin27.size(), 9);
				for (int kismin51 = 0; kismin51 < 4; kismin51++) kismin48.push_back(kismin47.size() >> 8 * kismin51);
				kismin48.insert(kismin48.end(), kismin47.begin(), kismin47.end());
				kismin48.insert(kismin48.end(), kismin49.begin(), kismin49.begin() + kismin50);
				kismin13 = 35;
			} else {
				kismin48.insert(kismin48.end(), kismin47.begin(), kismin47.end());
				kismin13 = 21;
			}
			kismin16.swap(kismin48);
			kismin15 = false;
		} else if (kismin13 == 21 || kismin13 == 35) {
			size_t kismin52 = kismin13 == 35 ? 6 : 2;
			uint32_t kismin53 = kismin13 == 35 ? (kismin16[2] | kismin16[3] << 8 | kismin16[4] << 16 | (uint32_t)kismin16[5] << 24) : kismin16.size() - 2;
			const uint8_t* kismin54 = &kismin16[kismin52];
			size_t kismin55 = (kismin53 >= 4 && kismin54[0] == 0xFF && kismin54[1] == 0xD9 && kismin54[2] == 0xFF && kismin54[3] == 0xD8) ? 4 : 0;
			int kismin56, kismin57, kismin58;
			uint8_t* kismin59 = stbi_load_from_memory(kismin54 + kismin55, kismin53 - kismin55, &kismin56, &kismin57, &kismin58, 0);
			if (kismin59) {
				Buf kismin60, kismin61;
				if (kismin58 == 4) {
					kismin60.resize((size_t)kismin56 * kismin57 * 3);
					kismin61.resize((size_t)kismin56 * kismin57);
					for (size_t kismin62 = 0; kismin62 < (size_t)kismin56 * kismin57; kismin62++) {
						uint8_t kismin63 = kismin59[kismin62 * 4 + 3];
						kismin61[kismin62] = kismin63;
						kismin60[kismin62 * 3] = (kismin59[kismin62 * 4] * kismin63 + 127) / 255;
						kismin60[kismin62 * 3 + 1] = (kismin59[kismin62 * 4 + 1] * kismin63 + 127) / 255;
						kismin60[kismin62 * 3 + 2] = (kismin59[kismin62 * 4 + 2] * kismin63 + 127) / 255;
					}
					kismin58 = 3;
				}
				if (kismin61.empty() && kismin13 == 35 && kismin52 + kismin53 < kismin16.size()) {
					kismin61.resize((size_t)kismin56 * kismin57);
					uLongf kismin64 = kismin61.size();
					uncompress(kismin61.data(), &kismin64, &kismin16[kismin52 + kismin53], kismin16.size() - kismin52 - kismin53);
				}
				bool kismin65 = false;
				if (!kismin61.empty()) {
					if (kismin4 >= 2) for (uint8_t& kismin66 : kismin61) kismin66 = (kismin66 * (kismin4 - 1) + 127) / 255 * 255 / (kismin4 - 1);
					for (uint8_t kismin67 : kismin61) if (kismin67 != 255) { kismin65 = true; break; }
				}
				jpge::params kismin68;
				kismin68.m_quality = kismin3;
				kismin68.m_subsampling = kismin58 == 1 ? jpge::Y_ONLY : jpge::H2V2;
				kismin68.m_two_pass_flag = true;
				int kismin69 = std::max(4096, kismin56 * kismin57 * kismin58 + 65536);
				Buf kismin70(kismin69);
				jpge::compress_image_to_jpeg_file_in_memory(kismin70.data(), kismin69, kismin56, kismin57, kismin58, kismin60.empty() ? kismin59 : kismin60.data(), kismin68);
				kismin70.resize(kismin69);
				stbi_image_free(kismin59);
				Buf kismin71(kismin16.begin(), kismin16.begin() + 2);
				if (kismin65) {
					Buf kismin72(compressBound(kismin61.size()));
					uLongf kismin73 = kismin72.size();
					compress2(kismin72.data(), &kismin73, kismin61.data(), kismin61.size(), 9);
					for (int kismin74 = 0; kismin74 < 4; kismin74++) kismin71.push_back(kismin70.size() >> 8 * kismin74);
					kismin71.insert(kismin71.end(), kismin70.begin(), kismin70.end());
					kismin71.insert(kismin71.end(), kismin72.begin(), kismin72.begin() + kismin73);
					kismin13 = 35;
				} else {
					kismin71.insert(kismin71.end(), kismin70.begin(), kismin70.end());
					kismin13 = 21;
				}
				kismin16.swap(kismin71);
				kismin15 = false;
			}
		}

		kismin15 = kismin15 || kismin16.size() >= 63;
		int kismin75 = kismin13 << 6 | (kismin15 ? 63 : (int)kismin16.size());
		kismin12.push_back(kismin75 & 255);
		kismin12.push_back(kismin75 >> 8);
		if (kismin15) for (int kismin76 = 0; kismin76 < 4; kismin76++) kismin12.push_back(kismin16.size() >> 8 * kismin76);
		kismin12.insert(kismin12.end(), kismin16.begin(), kismin16.end());
		if (kismin13 == 0) break;
	}
	kismin12.insert(kismin12.end(), kismin8.begin() + kismin11, kismin8.end());

	Buf kismin77 = {'C', 'W', 'S', kismin7[3], 0, 0, 0, 0};
	for (int kismin78 = 0; kismin78 < 4; kismin78++) kismin77[4 + kismin78] = (kismin12.size() + 8) >> 8 * kismin78;
	Buf kismin79(compressBound(kismin12.size()));
	uLongf kismin80 = kismin79.size();
	compress2(kismin79.data(), &kismin80, kismin12.data(), kismin12.size(), 9);
	kismin77.insert(kismin77.end(), kismin79.begin(), kismin79.begin() + kismin80);
	kismin5 = fopen(kismin2[2], "wb");
	fwrite(kismin77.data(), 1, kismin77.size(), kismin5);
	fclose(kismin5);
	return 0;
}
