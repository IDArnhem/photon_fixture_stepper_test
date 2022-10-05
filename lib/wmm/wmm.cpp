#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include "wmm.h"

#define PI_CONST 3.14159265359f
#define RADIANS_TO_DEGREES 0.017453292f
#define DEGREES_TO_RADIANS (PI_CONST / 180.0f)
#define A_CONST 6378.137f
#define A2_CONST (A_CONST * A_CONST)
#define B_CONST 6356.7523142f
#define B2_CONST (B_CONST * B_CONST)
#define RE_CONST 6371.2f
#define A4_CONST (A2_CONST * A2_CONST)
#define B4_CONST (B2_CONST * B2_CONST)
#define C2_CONST (A2_CONST - B2_CONST)
#define C4_CONST (A4_CONST - B4_CONST)
#define COEFFICIENTS_COUNT		90U

static float c[13][13];
static float cd[13][13];
static float k[13][13];
static float snorm[169];
static float fn[13];
static float fm[13];

const uint8_t wmm_cof_entries_encoded[] =
{0xDD, 0xF2, 0x23, 0x00, 0x83, 0x01, 0x00, 0xEB, 0xE2, 0x01, 0x81, 0xD7, 0x05, 0x8D, 0x01, 0xFB, 0x03, 0xE8, 0x86, 0x03, 0x00, 0xF3, 0x01, 0x00, 0xBC, 0xD1, 0x03, 0xDC, 0xD3, 0x03, 0xC7, 0x01, 0xEE, 0x04, 0x80, 0x86, 0x02, 0xF4, 0x72, 0x56, 0xEF, 0x03, 0x87, 0xD5, 0x01, 0x00, 0x1C, 0x00, 0xC2, 0xF4, 0x02, 0xF6, 0x0C, 0x7E, 0x39, 0x8A, 0xC1, 0x01, 0xB2, 0x25, 0x22, 0x4A, 0x89, 0x52, 0xF5, 0x54, 0xFA, 0x01, 0x0B, 0x87, 0x8D, 0x01, 0x00, 0x4B, 0x00, 0x9E, 0x7E, 0x84, 0x2C, 0x50, 0x02, 0x9E, 0x0D, 0xF0, 0x18, 0x7C, 0x85, 0x01, 0xD6, 0x30, 0x8E, 0x1F, 0x36, 0x25, 0x9F, 0x07, 0xED, 0x36, 0x77, 0x78, 0xE8, 0x24, 0x00, 0x43, 0x00, 0xAF, 0x38, 0x9D, 0x07, 0x06, 0x01, 0x96, 0x1D, 0xA4, 0x20, 0x47, 0x19, 0xFF, 0x15, 0xFD, 0x12, 0x01, 0x49, 0xE8, 0x17, 0x82, 0x05, 0x0C, 0x1E, 0x89, 0x02, 0x9F, 0x0F, 0x0A, 0x05, 0x93, 0x0A, 0x00, 0x46, 0x00, 0x90, 0x0A, 0xFF, 0x02, 0x44, 0x01, 0x9A, 0x0B, 0xBA, 0x03, 0x05, 0x52, 0xFF, 0x12, 0x8F, 0x08, 0x0E, 0x4E, 0xEA, 0x05, 0xC4, 0x0A, 0x4E, 0x09, 0x87, 0x02, 0x9A, 0x01, 0x00, 0x01, 0xC7, 0x0A, 0xA9, 0x0A, 0x08, 0x0A, 0xA6, 0x0C, 0x00, 0x41, 0x00, 0xC0, 0x0C, 0xC2, 0x08, 0x43, 0x05, 0xD3, 0x01, 0xE8, 0x02, 0x41, 0x06, 0xB5, 0x08, 0x17, 0x07, 0x47, 0x9E, 0x02, 0xAB, 0x03, 0x02, 0x42, 0x80, 0x01, 0x56, 0x45, 0x4C, 0xC8, 0x01, 0xD0, 0x04, 0x48, 0x02, 0xA2, 0x01, 0x53, 0x0A, 0x03, 0xAC, 0x03, 0x00, 0x41, 0x00, 0xA2, 0x01, 0x94, 0x01, 0x01, 0x43, 0xEF, 0x02, 0xD9, 0x02, 0x41, 0x07, 0x44, 0x80, 0x02, 0x05, 0x42, 0xD3, 0x03, 0xF6, 0x01, 0x41, 0x05, 0x99, 0x02, 0x95, 0x02, 0x04, 0x43, 0x89, 0x02, 0x24, 0x05, 0x45, 0xE5, 0x02, 0xC5, 0x01, 0x00, 0x04, 0x43, 0x1C, 0x04, 0x01, 0x32, 0x00, 0x41, 0x00, 0x92, 0x01, 0xE9, 0x03, 0x42, 0x43, 0x1D, 0xAF, 0x01, 0x00, 0x02, 0x4E, 0xA2, 0x01, 0x04, 0x44, 0x4B, 0x73, 0x43, 0x04, 0xC5, 0x02, 0x7E, 0x00, 0x01, 0x0B, 0x8E, 0x01, 0x03, 0x00, 0x99, 0x01, 0x04, 0x00, 0x42, 0xDD, 0x01, 0x4F, 0x00, 0x05, 0xF7, 0x01, 0xA1, 0x01, 0x44, 0x02, 0x53, 0x00, 0x00, 0x00, 0x7E, 0x22, 0x00, 0x00, 0x41, 0x42, 0x00, 0x01, 0x11, 0x23, 0x02, 0x43, 0x49, 0x30, 0x41, 0x01, 0x06, 0xD6, 0x01, 0x42, 0x42, 0x49, 0x41, 0x00, 0x01, 0x13, 0x6A, 0x41, 0x00, 0x0E, 0x62, 0x42, 0x41, 0x58, 0x41, 0x41, 0x02, 0x67, 0xD8, 0x01, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 0x4E, 0x00, 0x41, 0x00, 0x59, 0x1A, 0x00, 0x01, 0x18, 0x45, 0x00, 0x00, 0x49, 0x44, 0x00, 0x02, 0x03, 0x06, 0x41, 0x00, 0x47, 0x42, 0x00, 0x00, 0x41, 0x51, 0x00, 0x01, 0x0E, 0x50, 0x41, 0x00, 0x46, 0x5E, 0x41, 0x41, 0x02, 0x54, 0x41, 0x00, 0x1F, 0x5A, 0x41, 0x00, 0x54, 0x00, 0x00, 0x00, 0x41, 0x4C, 0x00, 0x00, 0x05, 0x05, 0x00, 0x00, 0x0D, 0x0D, 0x00, 0x41, 0x4C, 0x52, 0x00, 0x01, 0x07, 0x01, 0x00, 0x00, 0x03, 0x07, 0x00, 0x00, 0x05, 0x41, 0x00, 0x00, 0x42, 0x06, 0x00, 0x01, 0x45, 0x02, 0x00, 0x00, 0x01, 0x49, 0x00, 0x00, 0x4B, 0x00, 0x00, 0x00, 0x43, 0x05, 0x41, 0x41};

static wmm_cof_record_t wmm_cof_entries[COEFFICIENTS_COUNT];

static float convert_varint_to_float(char **bytes);

float wmm_get_date(uint8_t year, uint8_t month, uint8_t date)
{
	return (float)year + 2000.0f + (float)(month - 1U) / 12.0f + (float)(date - 1U) / (365.0f);
}

static float convert_varint_to_float(char **bytes)
{
	float result;
	int32_t result_int;
	bool negative = false;
	bool first_byte = true;
	uint8_t shift;

	do
	{
		if (first_byte)
		{
			if (**bytes & 0x40)
			{
				negative = true;
			}

			result_int = **bytes & 0x3f;
			shift = 6U;
			first_byte = false;
		}
		else
		{
			result_int += (uint32_t)(**bytes & 0x7f) << shift;
			shift += 7U;
		}

		if ((**bytes & 0x80) == 0U)
		{
			(*bytes)++;
			break;
		}

		(*bytes)++;

	} while (true);


	result = ((float)result_int) / 10.0f;
	if (negative)
	{
		result = -result;
	}

	return result;
}

void wmm_init(void)
{
	uint8_t j;
	uint8_t m;
	uint8_t n;
	uint8_t D2;
	float gnm;
	float hnm;
	float dgnm;
	float dhnm;
	float flnmj;
	uint8_t i;
	char *bytes = (char *)&wmm_cof_entries_encoded[0];

	// unpack coefficients
	for (i = 0U; i < COEFFICIENTS_COUNT; i++)
	{
		wmm_cof_entries[i].gnm = convert_varint_to_float(&bytes);
		wmm_cof_entries[i].hnm = convert_varint_to_float(&bytes);
		wmm_cof_entries[i].dgnm = convert_varint_to_float(&bytes);
		wmm_cof_entries[i].dhnm = convert_varint_to_float(&bytes);
	}

	c[0][0] = 0.0f;
	cd[0][0] = 0.0f;

	j = 0U;
	for (n = 1U; n <= 12U; n++)
	{
		for (m = 0U; m <= n; m++)
		{
			gnm = wmm_cof_entries[j].gnm;
			hnm = wmm_cof_entries[j].hnm;
			dgnm = wmm_cof_entries[j].dgnm;
			dhnm = wmm_cof_entries[j].dhnm;
			j++;

			if (m <= n)
			{
				c[m][n] = gnm;
				cd[m][n] = dgnm;
				if (m != 0U)
				{
					c[n][m - 1U] = hnm;
					cd[n][m - 1U] = dhnm;
				}
			}
		}
	}

	// CONVERT SCHMIDT NORMALIZED GAUSS COEFFICIENTS TO UNNORMALIZED
	*snorm = 1.0f;
	for (n = 1U; n <= 12U; n++)
	{
		*(snorm + n) = *(snorm + n - 1U) * (float)(2U * n - 1U) / (float)n;
		j = 2U;
		m = 0U;
		for (D2 = n - m + 1U; D2 > 0U; D2--)
		{
			k[m][n] = (float)(((n - 1U) * (n - 1U)) - (m * m)) / (float)((2U * n - 1U) * (2U * n - 3U));
			if (m > 0U)
			{
				flnmj = (float)((n - m + 1U) * j) / (float)(n + m);
				*(snorm + n + m * 13U) = *(snorm + n + (m - 1U) * 13U) * sqrt(flnmj);
				j = 1U;
				c[n][m - 1U] = *(snorm + n + m * 13U) * c[n][m - 1U];
				cd[n][m - 1U] = *(snorm + n + m * 13U) * cd[n][m - 1U];
			}
			c[m][n] = *(snorm + n + m * 13U) * c[m][n];
			cd[m][n] = *(snorm + n + m *13U) * cd[m][n];
			m += 1U;
	    }
		fn[n] = (float)(n + 1U);
		fm[n] = (float)n;
	}
	k[1][1] = 0.0f;
}

void E0000(float glat, float glon, float time_years, float *dec)
{
	static float tc[13][13];
	static float sp[13];
	static float cp[13];
	static float dp[13][13];
	static float pp[13];
	float dt = time_years - WMM_EPOCH;
	float rlon = glon * DEGREES_TO_RADIANS;
	float rlat = glat * DEGREES_TO_RADIANS;
	float srlon = sinf(rlon);
	float srlat = sinf(rlat);
	float crlon = cosf(rlon);
	float crlat = cosf(rlat);
	float srlat2 = srlat * srlat;
	float crlat2 = crlat * crlat;
	sp[0] = 0.0f;
	sp[1] = srlon;
	cp[0] = 1.0f;
	cp[1] = crlon;
	dp[0][0] = 0.0f;
	pp[0] = 1.0f;

	// CONVERT FROM GEODETIC COORDS. TO SPHERICAL COORDS
	float q = sqrtf(A2_CONST - C2_CONST * srlat2);
	float q2 = (A2_CONST / (B2_CONST)) * (A2_CONST / B2_CONST);
	float ct = srlat / sqrtf(q2 * crlat2 + srlat2);
	float st = sqrtf(1.0f - (ct * ct));
	float r2 = (A4_CONST - C4_CONST * srlat2) / (q * q);
	float r = sqrtf(r2);
	float d = sqrtf(A2_CONST * crlat2 + B2_CONST * srlat2);
	float ca = d / r;
	float sa = C2_CONST * crlat * srlat / (r * d);
	for (uint8_t m = 2U; m <= 12U; m++)
	{
		sp[m] = sp[1] * cp[m - 1U] + cp[1] * sp[m - 1U];
		cp[m] = cp[1] * cp[m - 1U] - sp[1] * sp[m - 1U];
	}
	float aor = RE_CONST / r;
	float ar = aor * aor;
	float br = 0.0f;
	float bt = 0.0f;
	float bp = 0.0f;
	float bpp = 0.0f;

	for (uint16_t n = 1U; n <= 12U; n++)
    {
		ar = ar * aor;
		uint8_t m = 0U;
		for (uint8_t D4 = n + 1U; D4 > 0U; D4--)
		{
			// COMPUTE UNNORMALIZED ASSOCIATED LEGENDRE POLYNOMIALS AND DERIVATIVES VIA RECURSION RELATIONS
			if (n == m)
			{
				*(snorm + n + m * 13U) = st * *(snorm + n - 1U + (m - 1U) * 13U);
				dp[m][n] = st * dp[m - 1U][n - 1U] + ct * *(snorm + n - 1U + (m - 1U) * 13U);
				goto S50;
			}
			if (n == 1U && m == 0U)
			{
				*(snorm + n + m * 13U) = ct * *(snorm + n - 1U + m * 13U);
				dp[m][n] = ct * dp[m][n - 1U] - st * *(snorm + n - 1U + m * 13U);
				goto S50;
			}
			if (n > 1U && n != m)
			{
				if (m > n - 2U)
				{
					*(snorm + n - 2U + m * 13U) = 0.0f;
				}
				if (m > n - 2U)
				{
					dp[m][n - 2U] = 0.0f;
				}
				*(snorm + n + m * 13U) = ct * *(snorm + n - 1U + m * 13U) - k[m][n] * *(snorm + n - 2U + m * 13U);
				dp[m][n] = ct * dp[m][n - 1U] - st * *(snorm + n - 1U + m * 13U) - k[m][n] * dp[m][n - 2U];
			}
        S50:

			// TIME ADJUST THE GAUSS COEFFICIENTS
			tc[m][n] = c[m][n] + dt * cd[m][n];
			if (m != 0U)
			{
				tc[n][m - 1U] = c[n][m - 1U] + dt * cd[n][m - 1U];
			}

			// ACCUMULATE TERMS OF THE SPHERICAL HARMONIC EXPANSIONS
			float par = ar * *(snorm + n + m * 13U);
			float temp1;
			float temp2;

			if (m == 0)
			{
				temp1 = tc[m][n] * cp[m];
				temp2 = tc[m][n] * sp[m];
			}
			else
			{
				temp1 = tc[m][n] * cp[m] + tc[n][m - 1U] * sp[m];
				temp2 = tc[m][n] * sp[m] - tc[n][m - 1U] * cp[m];
			}

			bt = bt - ar * temp1 * dp[m][n];
			bp += (fm[m] * temp2 * par);
			br += (fn[n] * temp1 * par);

			// SPECIAL CASE: NORTH/SOUTH GEOGRAPHIC POLES
			if (st == 0.0f && m == 1U)
			{
				if (n == 1U)
				{
					pp[n] = pp[n - 1U];
				}
				else
				{
					pp[n] = ct * pp[n - 1U] - k[m][n] * pp[n - 2U];
				}
				bpp += (fm[m] * temp2 * ar * pp[n]);
			}
			m += 1U;
        }
    }
	if (st == 0.0f)
	{
		bp = bpp;
	}
	else
	{
		bp /= st;
	}

	// ROTATE MAGNETIC VECTOR COMPONENTS FROM SPHERICAL TO GEODETIC COORDINATES
	float bx = -bt * ca - br * sa;
	float by = bp;

	// COMPUTE DECLINATION
	*dec = atan2f(by, bx) / DEGREES_TO_RADIANS;
}
