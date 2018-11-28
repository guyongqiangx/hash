#ifndef TA_SHA1_H
#define TA_SHA1_H

/*
 * This UUID is generated with uuidgen
 * the ITU-T UUID generator at http://www.itu.int/ITU-T/asn1/uuid.html
 */

/*
 * 82071ab3-f3e2-4dac-bf8c-bfc14f039ada
 */
#define TA_SHA1_UUID \
	{ 0x82071ab3, 0xf3e2, 0x4dac, \
		{ 0xbf, 0x8c, 0xbf, 0xc1, 0x4f, 0x03, 0x9a, 0xda} }

/*
 * The function IDs implemented in this TA
 */
#define TA_SHA1_CMD_INIT    0x10
#define TA_SHA1_CMD_UPDATE  0x20
#define TA_SHA1_CMD_FINAL   0x30

#endif