/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rsip_primitive.h"
#include "r_rsip_reg.h"
#include "r_rsip_util.h"

/***********************************************************************************************************************
 * Functions
 **********************************************************************************************************************/

rsip_ret_t r_rsip_pe2 (const uint32_t InData_PubKeyType[],
                       const uint32_t InData_PubKey[],
                       const uint32_t InData_KeyIndex[],
                       const uint32_t InData_DomainParam[],
                       uint32_t       OutData_EncSecret[])
{
    uint32_t OFS_ADR = 0U;
    uint32_t iLoop   = 0U;
    (void) OFS_ADR;

    if (RD1_MASK(REG_14B8H, 0x0000001eU) != 0)
    {
        return RSIP_RET_RESOURCE_CONFLICT;
    }

    WR1_PROG(REG_1B00H, 0x00e20001U);
    WR1_PROG(REG_144CH, 0x00000000U);

    r_rsip_func100(bswap_32big(0x62a332a1U), bswap_32big(0x3160310dU), bswap_32big(0x637f4c10U), bswap_32big(0x48950c08U));
    WR1_PROG(REG_1000H, 0x00010000U);
    WR1_PROG(REG_1024H, 0x000007f0U);

    WR1_PROG(REG_1600H, 0x00000b5aU);

    WR1_PROG(REG_1600H, 0x00000b9cU);

    OFS_ADR = 128;
    WR1_PROG(REG_1404H, 0x10200000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);
    WR1_PROG(REG_1404H, 0x10700000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);
    WR1_PROG(REG_1404H, 0x13400000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);
    WR1_PROG(REG_1404H, 0x17f00000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);
    WR1_PROG(REG_1404H, 0x18400000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);
    WR1_PROG(REG_1404H, 0x18900000U);
    WR1_PROG(REG_1400H, 0x00c00051U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);

    r_rsip_func070(InData_DomainParam);

    WR1_PROG(REG_1010H, 0x00000018U);
    WR1_PROG(REG_101CH, 0x00000070U);

    WR1_PROG(REG_1004H, 0x04040010U);
    WR1_PROG(REG_1000H, 0x00010001U);
    WAIT_STS(REG_1000H, 0, 0);

    WR1_PROG(REG_1444H, 0x000000c7U);
    WR1_PROG(REG_1608H, 0x800100e0U);
    WAIT_STS(REG_1444H, 31, 1);
    WR1_PROG(REG_1420H, InData_KeyIndex[0]);
    WR1_PROG(REG_1458H, 0x00000000U);

    WR1_PROG(REG_1444H, 0x000000a7U);
    WR1_PROG(REG_1608H, 0x800103a0U);
    WAIT_STS(REG_1444H, 31, 1);
    WR1_PROG(REG_1420H, bswap_32big(0x0000e201U));
    WR1_PROG(REG_1458H, 0x00000000U);

    r_rsip_func101(bswap_32big(0xd3ba51caU), bswap_32big(0xb5f5d6d8U), bswap_32big(0x1b9e3d68U), bswap_32big(0x63a3ca44U));
    r_rsip_func043();

    WR1_PROG(REG_1600H, 0x0000b4e0U);
    WR1_PROG(REG_1600H, 0x00000017U);

    WR1_PROG(REG_1444H, 0x000000a7U);
    WR1_PROG(REG_1608H, 0x800103a0U);
    WAIT_STS(REG_1444H, 31, 1);
    WR1_PROG(REG_1420H, bswap_32big(0x0000e201U));
    WR1_PROG(REG_1458H, 0x00000000U);

    r_rsip_func101(bswap_32big(0x29e98315U), bswap_32big(0xf1ca757bU), bswap_32big(0x070dfe26U), bswap_32big(0x823c9211U));
    r_rsip_func044();

    WR1_PROG(REG_1444H, 0x000007c2U);
    WR1_PROG(REG_1A2CH, 0x40000100U);
    WR1_PROG(REG_1A24H, 0xf7009d07U);

    for (iLoop = 0U; iLoop < 8U; )
    {
        WAIT_STS(REG_1444H, 31, 1);
        WR4_ADDR(REG_1420H, &InData_KeyIndex[1 + iLoop]);
        iLoop = iLoop + 4U;
    }

    WR1_PROG(REG_1458H, 0x00000000U);

    WR1_PROG(REG_1404H, 0x13200000U);
    WR1_PROG(REG_1400H, 0x00c20021U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);

    WR1_PROG(REG_1444H, 0x000003c2U);
    WR1_PROG(REG_1A2CH, 0x40000000U);
    WR1_PROG(REG_1A24H, 0x07008d05U);
    WAIT_STS(REG_1444H, 31, 1);
    WR4_ADDR(REG_1420H, &InData_KeyIndex[1 + iLoop]);

    WR1_PROG(REG_1A24H, 0x9c100005U);
    WR1_PROG(REG_1400H, 0x00820011U);
    WAIT_STS(REG_1404H, 30, 0);
    WR1_PROG(REG_143CH, 0x00001800U);

    r_rsip_func100(bswap_32big(0x63938360U), bswap_32big(0xdf7b7f55U), bswap_32big(0x3b1a44a4U), bswap_32big(0x8e6efdbcU));
    WR1_PROG(REG_143CH, 0x00400000U);

    if (CHCK_STS(REG_143CH, 22, 1))
    {
        r_rsip_func102(bswap_32big(0x2f393c7cU), bswap_32big(0x8c5e46b7U), bswap_32big(0xffe3e7e5U), bswap_32big(0x50c0fe61U));
        WR1_PROG(REG_14B8H, 0x00000040U);
        WAIT_STS(REG_142CH, 12, 0);

        return RSIP_RET_KEY_FAIL;
    }
    else
    {
        WR1_PROG(REG_1404H, 0x10c00000U);
        WR1_PROG(REG_1400H, 0x00c00051U);
        WAIT_STS(REG_1404H, 30, 0);
        WR1_PROG(REG_143CH, 0x00001800U);
        WR1_PROG(REG_1404H, 0x11100000U);
        WR1_PROG(REG_1400H, 0x00c00051U);
        WAIT_STS(REG_1404H, 30, 0);
        WR1_PROG(REG_143CH, 0x00001800U);

        WR1_PROG(REG_1444H, 0x000000c7U);
        WR1_PROG(REG_1608H, 0x80010360U);
        WAIT_STS(REG_1444H, 31, 1);
        WR1_PROG(REG_1420H, InData_PubKeyType[0]);
        WR1_PROG(REG_1458H, 0x00000000U);

        WR1_PROG(REG_1600H, 0x38000f7bU);
        WR1_PROG(REG_1608H, 0x00000080U);
        WR1_PROG(REG_143CH, 0x00260000U);

        r_rsip_func100(bswap_32big(0xe9d1e6ddU), bswap_32big(0x94ec31a1U), bswap_32big(0x742e2272U), bswap_32big(0x5616ad7bU));
        WR1_PROG(REG_143CH, 0x00400000U);

        if (CHCK_STS(REG_143CH, 22, 1))
        {
            WR1_PROG(REG_1444H, 0x000007d0U);
            WAIT_STS(REG_1444H, 31, 1);
            for (iLoop = 0U; iLoop < 32U; )
            {
                WR1_ADDR((&(REG_00F0H))[iLoop / 4], &InData_PubKey[0 + (iLoop / 4)]);
                iLoop = iLoop + 4U;
            }

            WR1_PROG(REG_1444H, 0x000007d0U);
            WAIT_STS(REG_1444H, 31, 1);
            for (iLoop = 0U; iLoop < 32U; )
            {
                WR1_ADDR((&(REG_0140H))[iLoop / 4], &InData_PubKey[8 + (iLoop / 4)]);
                iLoop = iLoop + 4U;
            }

            r_rsip_func101(bswap_32big(0x82c62168U), bswap_32big(0x93e2cc3eU), bswap_32big(0x8e3ad316U), bswap_32big(0xf7793701U));
        }
        else
        {
            WR1_PROG(REG_1444H, 0x000000c7U);
            WR1_PROG(REG_1608H, 0x800100e0U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, InData_PubKey[0]);
            WR1_PROG(REG_1458H, 0x00000000U);

            WR1_PROG(REG_1444H, 0x000000a7U);
            WR1_PROG(REG_1608H, 0x800103a0U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x0000e202U));
            WR1_PROG(REG_1458H, 0x00000000U);

            r_rsip_func101(bswap_32big(0x455c5100U), bswap_32big(0x494ce59fU), bswap_32big(0xaaa38b09U), bswap_32big(0x4fe63697U));
            r_rsip_func043();

            WR1_PROG(REG_1600H, 0x0000b4e0U);
            WR1_PROG(REG_1600H, 0x00000016U);

            WR1_PROG(REG_1444H, 0x000000a7U);
            WR1_PROG(REG_1608H, 0x800103a0U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x0000e202U));
            WR1_PROG(REG_1458H, 0x00000000U);

            r_rsip_func101(bswap_32big(0x0ef73082U), bswap_32big(0x164ea1c3U), bswap_32big(0xfb4e45a7U), bswap_32big(0x5fac691cU));
            r_rsip_func044();

            WR1_PROG(REG_1444H, 0x000000a2U);
            WR1_PROG(REG_1A24H, 0x08000044U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x00000000U));

            WR1_PROG(REG_1444H, 0x00000fc2U);
            WR1_PROG(REG_1A2CH, 0x40000300U);
            WR1_PROG(REG_1A24H, 0xe8009107U);

            WAIT_STS(REG_1444H, 31, 1);
            WR4_ADDR(REG_1420H, &InData_PubKey[1]);
            WAIT_STS(REG_1444H, 31, 1);
            WR4_ADDR(REG_1420H, &InData_PubKey[5]);

            WR1_PROG(REG_1404H, 0x10f00000U);
            WR1_PROG(REG_1400H, 0x00c20021U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            WAIT_STS(REG_1444H, 31, 1);
            WR4_ADDR(REG_1420H, &InData_PubKey[9]);
            WAIT_STS(REG_1444H, 31, 1);
            WR4_ADDR(REG_1420H, &InData_PubKey[13]);

            WR1_PROG(REG_1404H, 0x11400000U);
            WR1_PROG(REG_1400H, 0x00c20021U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            WR1_PROG(REG_1444H, 0x000003c2U);
            WR1_PROG(REG_1A2CH, 0x40000000U);
            WR1_PROG(REG_1A24H, 0x07008d05U);
            WAIT_STS(REG_1444H, 31, 1);
            WR4_ADDR(REG_1420H, &InData_PubKey[17]);

            WR1_PROG(REG_1A24H, 0x9c100005U);
            WR1_PROG(REG_1400H, 0x00820011U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            r_rsip_func100(bswap_32big(0x74c3157dU), bswap_32big(0x2dcea5d2U), bswap_32big(0x459809c9U), bswap_32big(0xe0c6936dU));
            WR1_PROG(REG_143CH, 0x00400000U);

            if (CHCK_STS(REG_143CH, 22, 1))
            {
                r_rsip_func102(bswap_32big(0x26949b41U), bswap_32big(0x8f92608cU), bswap_32big(0x762fbd89U), bswap_32big(0xe14f434aU));
                WR1_PROG(REG_14B8H, 0x00000040U);
                WAIT_STS(REG_142CH, 12, 0);

                return RSIP_RET_KEY_FAIL;
            }
            else
            {
                r_rsip_func101(bswap_32big(0xb65fc77cU), bswap_32big(0x1d9908beU), bswap_32big(0xaf7473a5U), bswap_32big(0xb78d6075U));
            }
        }

        WR1_PROG(REG_1404H, 0x11600000U);
        WR1_PROG(REG_1400H, 0x00c000f1U);
        WAIT_STS(REG_1404H, 30, 0);
        WR1_PROG(REG_143CH, 0x00001800U);

        WR1_PROG(REG_1014H, 0x00000110U);
        WR1_PROG(REG_1018H, 0x00000390U);
        WR1_PROG(REG_101CH, 0x00000070U);
        WR1_PROG(REG_1020H, 0x00000200U);

        WR1_PROG(REG_1004H, 0x04040002U);
        WR1_PROG(REG_1000H, 0x00010201U);
        WAIT_STS(REG_1000H, 0, 0);

        WR1_PROG(REG_1014H, 0x00000160U);
        WR1_PROG(REG_1018H, 0x00000390U);
        WR1_PROG(REG_101CH, 0x00000070U);
        WR1_PROG(REG_1020H, 0x00000228U);

        WR1_PROG(REG_1004H, 0x04040002U);
        WR1_PROG(REG_1000H, 0x00010201U);
        WAIT_STS(REG_1000H, 0, 0);

        WR1_PROG(REG_1444H, 0x000000a7U);
        WR1_PROG(REG_1608H, 0x800103a0U);
        WAIT_STS(REG_1444H, 31, 1);
        WR1_PROG(REG_1420H, bswap_32big(0x000000e2U));
        WR1_PROG(REG_1458H, 0x00000000U);

        r_rsip_func101(bswap_32big(0xa2fce6f5U), bswap_32big(0x779b2edeU), bswap_32big(0x2474de0cU), bswap_32big(0x7ef5ec5aU));
        r_rsip_func088();

        r_rsip_func100(bswap_32big(0xa3f193d0U), bswap_32big(0x7466e56dU), bswap_32big(0x61ce36aaU), bswap_32big(0x543be37eU));
        WR1_PROG(REG_143CH, 0x00400000U);

        if (CHCK_STS(REG_143CH, 22, 1))
        {
            r_rsip_func102(bswap_32big(0xde1a2a6aU), bswap_32big(0x4ec6a4a7U), bswap_32big(0x36d8692bU), bswap_32big(0x2d325b9bU));
            WR1_PROG(REG_14B8H, 0x00000040U);
            WAIT_STS(REG_142CH, 12, 0);

            return RSIP_RET_FAIL;
        }
        else
        {
            r_rsip_func100(bswap_32big(0xb5049475U), bswap_32big(0xee3dbcb2U), bswap_32big(0x5b423d8eU), bswap_32big(0xae7c5b10U));
            r_rsip_func103();

            WR1_PROG(REG_1444H, 0x000000a2U);
            WR1_PROG(REG_1A24H, 0x0c200104U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x00000000U));

            WR1_PROG(REG_1608H, 0x80010000U);
            WR1_PROG(REG_1400H, 0x03420005U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);
            WR1_PROG(REG_1400H, 0x0002000dU);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            WR1_PROG(REG_1600H, 0x000034e0U);

            WR1_PROG(REG_1444H, 0x000000a7U);
            WR1_PROG(REG_1608H, 0x800103a0U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x0000e203U));
            WR1_PROG(REG_1458H, 0x00000000U);

            r_rsip_func101(bswap_32big(0x72faac16U), bswap_32big(0xda0661d9U), bswap_32big(0x6dd8fc91U), bswap_32big(0xb0ede0c4U));
            r_rsip_func043();

            WR1_PROG(REG_1600H, 0x0000b4e0U);
            WR1_PROG(REG_1600H, 0x015c5d71U);

            WR1_PROG(REG_1444H, 0x000000a7U);
            WR1_PROG(REG_1608H, 0x800103a0U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x0000e203U));
            WR1_PROG(REG_1458H, 0x00000000U);

            r_rsip_func101(bswap_32big(0x79d167f3U), bswap_32big(0x8a1752e6U), bswap_32big(0x847c11d8U), bswap_32big(0x1c158e46U));
            r_rsip_func044();

            r_rsip_func100(bswap_32big(0x9f24ee28U), bswap_32big(0xd0e30f82U), bswap_32big(0xad3713b7U), bswap_32big(0xf389361aU));
            WR1_PROG(REG_1A2CH, 0x40000100U);
            WR1_PROG(REG_1A24H, 0xe7009d07U);
            WR1_PROG(REG_1404H, 0x12800000U);
            WR1_PROG(REG_1400H, 0x00830021U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            WR1_PROG(REG_1408H, 0x00002022U);
            WAIT_STS(REG_1408H, 30, 1);
            RD4_ADDR(REG_1420H, &OutData_EncSecret[1]);
            WAIT_STS(REG_1408H, 30, 1);
            RD4_ADDR(REG_1420H, &OutData_EncSecret[5]);

            r_rsip_func100(bswap_32big(0x339850beU), bswap_32big(0x2cae0b73U), bswap_32big(0x1f03dbfdU), bswap_32big(0x9916c85cU));
            WR1_PROG(REG_1444H, 0x000000a2U);
            WR1_PROG(REG_1A24H, 0x0c000104U);
            WAIT_STS(REG_1444H, 31, 1);
            WR1_PROG(REG_1420H, bswap_32big(0x00000000U));

            WR1_PROG(REG_1A2CH, 0x40000000U);
            WR1_PROG(REG_1A24H, 0x09108105U);
            WR1_PROG(REG_1400H, 0x00820011U);
            WAIT_STS(REG_1404H, 30, 0);
            WR1_PROG(REG_143CH, 0x00001800U);

            WR1_PROG(REG_1408H, 0x00002012U);
            WAIT_STS(REG_1408H, 30, 1);
            RD4_ADDR(REG_1420H, &OutData_EncSecret[9]);

            r_rsip_func100(bswap_32big(0x593f37c3U), bswap_32big(0x3c220bd5U), bswap_32big(0x140748abU), bswap_32big(0xf426bf49U));
            WR1_PROG(REG_1608H, 0x81010000U);
            WR1_PROG(REG_1408H, 0x00005006U);
            WAIT_STS(REG_1408H, 30, 1);
            RD1_ADDR(REG_1420H, &OutData_EncSecret[0]);

            r_rsip_func102(bswap_32big(0xb566a837U), bswap_32big(0x07ccbe98U), bswap_32big(0x23fd0bfaU), bswap_32big(0x8cdd7025U));
            WR1_PROG(REG_14B8H, 0x00000040U);
            WAIT_STS(REG_142CH, 12, 0);

            return RSIP_RET_PASS;
        }
    }
}
