/*
* Copyright (c) 2020 - 2024 Renesas Electronics Corporation and/or its affiliates
*
* SPDX-License-Identifier: BSD-3-Clause
*/

/***********************************************************************************************************************
 * Includes
 **********************************************************************************************************************/
#include "r_rsip_primitive.h"
#include "r_rsip_util.h"

/***********************************************************************************************************************
 * Global variables
 **********************************************************************************************************************/

uint32_t S_RAM[RSIP_PRV_WORD_SIZE_S_RAM];
uint32_t S_HEAP[RSIP_PRV_WORD_SIZE_S_HEAP];
uint32_t const * S_INST2;
uint32_t INST_DATA_SIZE;
uint32_t KEY_INDEX_SIZE;
uint32_t WRAPPED_KEY_SIZE;

uint32_t const DomainParam_NIST_P192[] =
{
    BSWAP_32BIG_C(0x8d739360U), BSWAP_32BIG_C(0xfd7ede3fU), BSWAP_32BIG_C(0x669521c5U), BSWAP_32BIG_C(0xb8341cbbU),
    BSWAP_32BIG_C(0x140deb42U), BSWAP_32BIG_C(0x0c0a1fb4U), BSWAP_32BIG_C(0x531044d2U), BSWAP_32BIG_C(0xfacc28dcU),
    BSWAP_32BIG_C(0x85a2aafeU), BSWAP_32BIG_C(0x37eb9074U), BSWAP_32BIG_C(0xf7b22873U), BSWAP_32BIG_C(0xfbcc9317U),
    BSWAP_32BIG_C(0x863ef860U), BSWAP_32BIG_C(0xdeb83a05U), BSWAP_32BIG_C(0x81e89b95U), BSWAP_32BIG_C(0xe65a8fe9U),
    BSWAP_32BIG_C(0x62d5e768U), BSWAP_32BIG_C(0x7cda9337U), BSWAP_32BIG_C(0x5c94f169U), BSWAP_32BIG_C(0x45cb7864U),
    BSWAP_32BIG_C(0x36b0623bU), BSWAP_32BIG_C(0x2d4ef58aU), BSWAP_32BIG_C(0x224f19cdU), BSWAP_32BIG_C(0xe90c73a1U),
    BSWAP_32BIG_C(0xc45844d2U), BSWAP_32BIG_C(0xe82fa9b6U), BSWAP_32BIG_C(0xc58fbdebU), BSWAP_32BIG_C(0xce874ce4U),
    BSWAP_32BIG_C(0x3f5df995U), BSWAP_32BIG_C(0x362c67aeU), BSWAP_32BIG_C(0xf6d1af40U), BSWAP_32BIG_C(0xc722846eU),
    BSWAP_32BIG_C(0xae2b5b3cU), BSWAP_32BIG_C(0x09f8875cU), BSWAP_32BIG_C(0x7dae67c6U), BSWAP_32BIG_C(0x1d336db2U),
    BSWAP_32BIG_C(0x9a172c90U), BSWAP_32BIG_C(0x1b4eb189U), BSWAP_32BIG_C(0xedf86918U), BSWAP_32BIG_C(0xa9efc2f9U),
    BSWAP_32BIG_C(0x31f5f50eU), BSWAP_32BIG_C(0x717e7940U), BSWAP_32BIG_C(0x63fc2af0U), BSWAP_32BIG_C(0x98ecb038U),
    BSWAP_32BIG_C(0x1bcce632U), BSWAP_32BIG_C(0x5632930eU), BSWAP_32BIG_C(0xd42e731cU), BSWAP_32BIG_C(0x533b5c2dU),
    BSWAP_32BIG_C(0x3074d94dU), BSWAP_32BIG_C(0xb791dba2U), BSWAP_32BIG_C(0x5fe03498U), BSWAP_32BIG_C(0x884e5088U),
    BSWAP_32BIG_C(0x2eec4a56U), BSWAP_32BIG_C(0xc1fe2fa5U), BSWAP_32BIG_C(0x8a336c66U), BSWAP_32BIG_C(0xef926760U),
    BSWAP_32BIG_C(0xff460d6fU), BSWAP_32BIG_C(0x5c39ce75U), BSWAP_32BIG_C(0x33b99bdbU), BSWAP_32BIG_C(0x68d187d0U),
    BSWAP_32BIG_C(0x4a888212U), BSWAP_32BIG_C(0xfbdd931dU), BSWAP_32BIG_C(0x31f200deU), BSWAP_32BIG_C(0x2d0a66b6U),
    BSWAP_32BIG_C(0x05df0ad8U), BSWAP_32BIG_C(0xa865368fU), BSWAP_32BIG_C(0x65351baaU), BSWAP_32BIG_C(0x7c5aea69U),
    BSWAP_32BIG_C(0x0f3c5f06U), BSWAP_32BIG_C(0x62e498a1U), BSWAP_32BIG_C(0xa2d9d94dU), BSWAP_32BIG_C(0x58194cdaU),
};

uint32_t const DomainParam_NIST_P224[] =
{
    BSWAP_32BIG_C(0x0a1c9774U), BSWAP_32BIG_C(0x7dda462cU), BSWAP_32BIG_C(0x98395868U), BSWAP_32BIG_C(0x28cbc3c3U),
    BSWAP_32BIG_C(0x0a8df02aU), BSWAP_32BIG_C(0x9c8abdb0U), BSWAP_32BIG_C(0x6bee83a7U), BSWAP_32BIG_C(0xdde17559U),
    BSWAP_32BIG_C(0x94cfcb10U), BSWAP_32BIG_C(0xd926d09bU), BSWAP_32BIG_C(0x61eb5b97U), BSWAP_32BIG_C(0x9c2dad08U),
    BSWAP_32BIG_C(0xd232f14aU), BSWAP_32BIG_C(0x8fb72c2cU), BSWAP_32BIG_C(0xfc44051cU), BSWAP_32BIG_C(0x93280bf0U),
    BSWAP_32BIG_C(0x5463afe9U), BSWAP_32BIG_C(0x94b751dbU), BSWAP_32BIG_C(0xb7d5024aU), BSWAP_32BIG_C(0x177ce72aU),
    BSWAP_32BIG_C(0x18e951a6U), BSWAP_32BIG_C(0xb46fb71cU), BSWAP_32BIG_C(0x2186414bU), BSWAP_32BIG_C(0x19eb64b0U),
    BSWAP_32BIG_C(0x5ee1ee19U), BSWAP_32BIG_C(0x1ae0d537U), BSWAP_32BIG_C(0x2b2e6443U), BSWAP_32BIG_C(0x55613a15U),
    BSWAP_32BIG_C(0xbc092520U), BSWAP_32BIG_C(0xc70f8f5bU), BSWAP_32BIG_C(0xd1ef9455U), BSWAP_32BIG_C(0x26fb4e18U),
    BSWAP_32BIG_C(0x5bf0c079U), BSWAP_32BIG_C(0x080ca300U), BSWAP_32BIG_C(0x8ccece0bU), BSWAP_32BIG_C(0xa6886a21U),
    BSWAP_32BIG_C(0xb2c207f2U), BSWAP_32BIG_C(0xe323f8b7U), BSWAP_32BIG_C(0xfbd9e54aU), BSWAP_32BIG_C(0x43f5eaafU),
    BSWAP_32BIG_C(0xab94a327U), BSWAP_32BIG_C(0xcbdaf526U), BSWAP_32BIG_C(0xf3eaefd5U), BSWAP_32BIG_C(0x6647c5dbU),
    BSWAP_32BIG_C(0xf5f0dc46U), BSWAP_32BIG_C(0x859b555bU), BSWAP_32BIG_C(0x059b78b3U), BSWAP_32BIG_C(0x7d0219eaU),
    BSWAP_32BIG_C(0xab7735b5U), BSWAP_32BIG_C(0x957cfa54U), BSWAP_32BIG_C(0x8b6d9baeU), BSWAP_32BIG_C(0xc0b21767U),
    BSWAP_32BIG_C(0x859449acU), BSWAP_32BIG_C(0x8fc06db5U), BSWAP_32BIG_C(0x192a96dbU), BSWAP_32BIG_C(0xed6dcbadU),
    BSWAP_32BIG_C(0x110af69cU), BSWAP_32BIG_C(0x68d615d6U), BSWAP_32BIG_C(0x7c9c5436U), BSWAP_32BIG_C(0xc0ac8afeU),
    BSWAP_32BIG_C(0x28780209U), BSWAP_32BIG_C(0x9fc5d609U), BSWAP_32BIG_C(0x7eee921dU), BSWAP_32BIG_C(0xa984f93eU),
    BSWAP_32BIG_C(0x28f2539fU), BSWAP_32BIG_C(0x23d5c67eU), BSWAP_32BIG_C(0x62bba4abU), BSWAP_32BIG_C(0x75339f7bU),
    BSWAP_32BIG_C(0x801a6bdfU), BSWAP_32BIG_C(0xc59fd5eeU), BSWAP_32BIG_C(0x00e4ab2cU), BSWAP_32BIG_C(0xebbe5dbcU),
};

uint32_t const DomainParam_NIST_P256[] =
{
    BSWAP_32BIG_C(0x035ed43bU), BSWAP_32BIG_C(0x398dbc01U), BSWAP_32BIG_C(0x9109272cU), BSWAP_32BIG_C(0x3f7d6b72U),
    BSWAP_32BIG_C(0x28606f3cU), BSWAP_32BIG_C(0xdb667ff2U), BSWAP_32BIG_C(0x0d5aee59U), BSWAP_32BIG_C(0x2ae099c7U),
    BSWAP_32BIG_C(0xf7a3fb8eU), BSWAP_32BIG_C(0x9c6f1282U), BSWAP_32BIG_C(0x0c1cac92U), BSWAP_32BIG_C(0x85281624U),
    BSWAP_32BIG_C(0x46126296U), BSWAP_32BIG_C(0x8638b754U), BSWAP_32BIG_C(0x6c5bffc3U), BSWAP_32BIG_C(0x1c6f0d31U),
    BSWAP_32BIG_C(0x6f6e4db7U), BSWAP_32BIG_C(0x70c024c1U), BSWAP_32BIG_C(0x4c3f1aa2U), BSWAP_32BIG_C(0x075d1a79U),
    BSWAP_32BIG_C(0x2770976fU), BSWAP_32BIG_C(0x921ca8d1U), BSWAP_32BIG_C(0x5a912313U), BSWAP_32BIG_C(0x44814876U),
    BSWAP_32BIG_C(0x1adc96d6U), BSWAP_32BIG_C(0x3c969ec2U), BSWAP_32BIG_C(0x0dcdb2ddU), BSWAP_32BIG_C(0x212cc108U),
    BSWAP_32BIG_C(0xe82419d5U), BSWAP_32BIG_C(0x8342e6c2U), BSWAP_32BIG_C(0x6076dbe3U), BSWAP_32BIG_C(0xaedeeb8eU),
    BSWAP_32BIG_C(0x8d2c9fddU), BSWAP_32BIG_C(0x028d7dadU), BSWAP_32BIG_C(0xe9b5b8e5U), BSWAP_32BIG_C(0xd1e13e7bU),
    BSWAP_32BIG_C(0x723e3beaU), BSWAP_32BIG_C(0x9d2939c5U), BSWAP_32BIG_C(0x02a42453U), BSWAP_32BIG_C(0x17c7bc07U),
    BSWAP_32BIG_C(0x9027e99fU), BSWAP_32BIG_C(0x65352bd6U), BSWAP_32BIG_C(0x1bba4a2dU), BSWAP_32BIG_C(0xb31fe39cU),
    BSWAP_32BIG_C(0x2cbc4542U), BSWAP_32BIG_C(0xe9d4ce5fU), BSWAP_32BIG_C(0xfef4af1cU), BSWAP_32BIG_C(0x44bdf683U),
    BSWAP_32BIG_C(0xe1affc99U), BSWAP_32BIG_C(0x4fe934cfU), BSWAP_32BIG_C(0x77544cbcU), BSWAP_32BIG_C(0x9ff7d5efU),
    BSWAP_32BIG_C(0xe3604ebbU), BSWAP_32BIG_C(0x45f03e92U), BSWAP_32BIG_C(0xd258582dU), BSWAP_32BIG_C(0x9c807d86U),
    BSWAP_32BIG_C(0xbdc6a8d5U), BSWAP_32BIG_C(0xa3b21009U), BSWAP_32BIG_C(0x23725fbcU), BSWAP_32BIG_C(0x1d067998U),
    BSWAP_32BIG_C(0xda886eb9U), BSWAP_32BIG_C(0xb53796dbU), BSWAP_32BIG_C(0x2caba848U), BSWAP_32BIG_C(0xb371ebb0U),
    BSWAP_32BIG_C(0x607f2890U), BSWAP_32BIG_C(0x2c9f8d55U), BSWAP_32BIG_C(0x990ac425U), BSWAP_32BIG_C(0xebfadd24U),
    BSWAP_32BIG_C(0x2504fdaaU), BSWAP_32BIG_C(0xbad6b4b2U), BSWAP_32BIG_C(0x00f5cc25U), BSWAP_32BIG_C(0x45acaf5cU),
};

uint32_t const DomainParam_NIST_P384[] =
{
    BSWAP_32BIG_C(0x454baa1bU), BSWAP_32BIG_C(0x0e821ac1U), BSWAP_32BIG_C(0xdc20feeeU), BSWAP_32BIG_C(0xb83f8abdU),
    BSWAP_32BIG_C(0x0b450ba5U), BSWAP_32BIG_C(0xee2ad4efU), BSWAP_32BIG_C(0xcec80fc9U), BSWAP_32BIG_C(0x0672b50bU),
    BSWAP_32BIG_C(0x8c2f5f5fU), BSWAP_32BIG_C(0x17c1f268U), BSWAP_32BIG_C(0xe721d5dcU), BSWAP_32BIG_C(0x2af26798U),
    BSWAP_32BIG_C(0x090c3649U), BSWAP_32BIG_C(0xcefa2599U), BSWAP_32BIG_C(0xd05c3e1cU), BSWAP_32BIG_C(0x098ec55bU),
    BSWAP_32BIG_C(0x099e306bU), BSWAP_32BIG_C(0xc8862365U), BSWAP_32BIG_C(0x489507e8U), BSWAP_32BIG_C(0xc56d8e56U),
    BSWAP_32BIG_C(0x9f80068dU), BSWAP_32BIG_C(0xb04512acU), BSWAP_32BIG_C(0x6aaeb283U), BSWAP_32BIG_C(0x7a16d53eU),
    BSWAP_32BIG_C(0x0db27194U), BSWAP_32BIG_C(0x86260d17U), BSWAP_32BIG_C(0xab0da504U), BSWAP_32BIG_C(0x04048f61U),
    BSWAP_32BIG_C(0x0982635dU), BSWAP_32BIG_C(0x6bafe52aU), BSWAP_32BIG_C(0xc726ae28U), BSWAP_32BIG_C(0x7f6c25a4U),
    BSWAP_32BIG_C(0xe0d754b2U), BSWAP_32BIG_C(0xf926ceabU), BSWAP_32BIG_C(0x221eab95U), BSWAP_32BIG_C(0x0b094488U),
    BSWAP_32BIG_C(0x71b2ea1fU), BSWAP_32BIG_C(0xa1f96200U), BSWAP_32BIG_C(0x6eb6e3b1U), BSWAP_32BIG_C(0xa5139a61U),
    BSWAP_32BIG_C(0x57426fc5U), BSWAP_32BIG_C(0x2a7abbe7U), BSWAP_32BIG_C(0x5159a15aU), BSWAP_32BIG_C(0xd52fa7a4U),
    BSWAP_32BIG_C(0xfbb72d4aU), BSWAP_32BIG_C(0x3d55ee79U), BSWAP_32BIG_C(0xd1c56bbaU), BSWAP_32BIG_C(0xfef0ef1bU),
    BSWAP_32BIG_C(0x0debba5fU), BSWAP_32BIG_C(0x97177c11U), BSWAP_32BIG_C(0xe55fcc77U), BSWAP_32BIG_C(0xf4618e33U),
    BSWAP_32BIG_C(0x3d0f68e9U), BSWAP_32BIG_C(0x05c30c1bU), BSWAP_32BIG_C(0xabaae2d3U), BSWAP_32BIG_C(0x1615d192U),
    BSWAP_32BIG_C(0x0e4acc65U), BSWAP_32BIG_C(0x22a4ffc5U), BSWAP_32BIG_C(0xd5b03676U), BSWAP_32BIG_C(0x40946677U),
    BSWAP_32BIG_C(0xfacdc777U), BSWAP_32BIG_C(0x4a67f35cU), BSWAP_32BIG_C(0x80963009U), BSWAP_32BIG_C(0xe848cc42U),
    BSWAP_32BIG_C(0xe0d42183U), BSWAP_32BIG_C(0xaab1100cU), BSWAP_32BIG_C(0x4f4f5956U), BSWAP_32BIG_C(0x05528ef9U),
    BSWAP_32BIG_C(0x1e70a8d1U), BSWAP_32BIG_C(0x24d90221U), BSWAP_32BIG_C(0xe7789138U), BSWAP_32BIG_C(0xb4a227eaU),
    BSWAP_32BIG_C(0xfbdd2a0fU), BSWAP_32BIG_C(0x402cbab5U), BSWAP_32BIG_C(0x3b7f01a3U), BSWAP_32BIG_C(0x82f7e985U),
    BSWAP_32BIG_C(0x6e5ffffeU), BSWAP_32BIG_C(0x2c53055fU), BSWAP_32BIG_C(0xd0395451U), BSWAP_32BIG_C(0x4895233bU),
    BSWAP_32BIG_C(0xbb0ccecbU), BSWAP_32BIG_C(0x03f7d032U), BSWAP_32BIG_C(0x8fbc60fbU), BSWAP_32BIG_C(0x64f74639U),
    BSWAP_32BIG_C(0x4a7b85ffU), BSWAP_32BIG_C(0x06c0c155U), BSWAP_32BIG_C(0x882880f8U), BSWAP_32BIG_C(0x86ba7841U),
    BSWAP_32BIG_C(0xd8eed2a9U), BSWAP_32BIG_C(0xa70227daU), BSWAP_32BIG_C(0x58d9db68U), BSWAP_32BIG_C(0xd3400847U),
    BSWAP_32BIG_C(0xe906f73bU), BSWAP_32BIG_C(0x3e16537fU), BSWAP_32BIG_C(0x93e84a9fU), BSWAP_32BIG_C(0x24efbb5eU),
    BSWAP_32BIG_C(0xabde9092U), BSWAP_32BIG_C(0xfcab1f48U), BSWAP_32BIG_C(0x15b003f8U), BSWAP_32BIG_C(0xa3ffd73dU),
    BSWAP_32BIG_C(0x659cbeb7U), BSWAP_32BIG_C(0x5df7929aU), BSWAP_32BIG_C(0x08200ae3U), BSWAP_32BIG_C(0xece2c7ccU),
};

uint32_t const DomainParam_NIST_P521[] =
{
    BSWAP_32BIG_C(0x7a249f9aU), BSWAP_32BIG_C(0xa02b4d76U), BSWAP_32BIG_C(0x45ae598eU), BSWAP_32BIG_C(0xea6bad10U),
    BSWAP_32BIG_C(0x5796477cU), BSWAP_32BIG_C(0x2e42c734U), BSWAP_32BIG_C(0x46071abfU), BSWAP_32BIG_C(0x5dec4559U),
    BSWAP_32BIG_C(0xa25e4aa4U), BSWAP_32BIG_C(0xb1f05f07U), BSWAP_32BIG_C(0x9fa87f1cU), BSWAP_32BIG_C(0x81e7d466U),
    BSWAP_32BIG_C(0xb8ed9524U), BSWAP_32BIG_C(0x82de7b9cU), BSWAP_32BIG_C(0x93e73a2fU), BSWAP_32BIG_C(0xf52f0247U),
    BSWAP_32BIG_C(0xa76e9d6eU), BSWAP_32BIG_C(0x5e5deb97U), BSWAP_32BIG_C(0xe345ae73U), BSWAP_32BIG_C(0x12975194U),
    BSWAP_32BIG_C(0xe4be8c4eU), BSWAP_32BIG_C(0x99691e35U), BSWAP_32BIG_C(0xf1557973U), BSWAP_32BIG_C(0x829bd421U),
    BSWAP_32BIG_C(0xda47480bU), BSWAP_32BIG_C(0xb049a32eU), BSWAP_32BIG_C(0xd1de53a1U), BSWAP_32BIG_C(0xcb6ca7fcU),
    BSWAP_32BIG_C(0x32a5777dU), BSWAP_32BIG_C(0x268a501dU), BSWAP_32BIG_C(0x1bb6b947U), BSWAP_32BIG_C(0x9e56b5f2U),
    BSWAP_32BIG_C(0xff561b0cU), BSWAP_32BIG_C(0x47efd961U), BSWAP_32BIG_C(0x9c63770dU), BSWAP_32BIG_C(0xf5c4b79bU),
    BSWAP_32BIG_C(0x5eb8417cU), BSWAP_32BIG_C(0xbc7dd1dcU), BSWAP_32BIG_C(0xc06c8bc2U), BSWAP_32BIG_C(0x388f1042U),
    BSWAP_32BIG_C(0x93703d42U), BSWAP_32BIG_C(0x2ad272ecU), BSWAP_32BIG_C(0x50ecccd3U), BSWAP_32BIG_C(0x0443f647U),
    BSWAP_32BIG_C(0xcecf72aaU), BSWAP_32BIG_C(0xa98babd3U), BSWAP_32BIG_C(0x7cc05c6bU), BSWAP_32BIG_C(0x23c861b6U),
    BSWAP_32BIG_C(0x227cc559U), BSWAP_32BIG_C(0xe6288893U), BSWAP_32BIG_C(0x7806f2dbU), BSWAP_32BIG_C(0xcac45daeU),
    BSWAP_32BIG_C(0xdca55dd8U), BSWAP_32BIG_C(0x612fbc24U), BSWAP_32BIG_C(0xc0bb05cdU), BSWAP_32BIG_C(0xb95465c5U),
    BSWAP_32BIG_C(0x3817299cU), BSWAP_32BIG_C(0x3981162eU), BSWAP_32BIG_C(0x28baff24U), BSWAP_32BIG_C(0xc0e2e107U),
    BSWAP_32BIG_C(0x72e29fdfU), BSWAP_32BIG_C(0x90371d50U), BSWAP_32BIG_C(0x6d28466eU), BSWAP_32BIG_C(0xd3429dfaU),
    BSWAP_32BIG_C(0xf455e0faU), BSWAP_32BIG_C(0xec8eac53U), BSWAP_32BIG_C(0x59bf3157U), BSWAP_32BIG_C(0x56487a6dU),
    BSWAP_32BIG_C(0xcfa45250U), BSWAP_32BIG_C(0x4b3a84e9U), BSWAP_32BIG_C(0x761813e0U), BSWAP_32BIG_C(0x0b2dd569U),
    BSWAP_32BIG_C(0x113b79dfU), BSWAP_32BIG_C(0x966c60b7U), BSWAP_32BIG_C(0x0be5ccb7U), BSWAP_32BIG_C(0xd1d63f28U),
    BSWAP_32BIG_C(0xae0bd280U), BSWAP_32BIG_C(0x72306332U), BSWAP_32BIG_C(0x92b3f2d0U), BSWAP_32BIG_C(0xee95a228U),
    BSWAP_32BIG_C(0x55181fbaU), BSWAP_32BIG_C(0xdecd4077U), BSWAP_32BIG_C(0xeae51cecU), BSWAP_32BIG_C(0x9508c693U),
    BSWAP_32BIG_C(0xde21165dU), BSWAP_32BIG_C(0x11ce5305U), BSWAP_32BIG_C(0x4f3e737dU), BSWAP_32BIG_C(0x98190e93U),
    BSWAP_32BIG_C(0xf6598bbdU), BSWAP_32BIG_C(0x047fbd98U), BSWAP_32BIG_C(0xf3a5a7c6U), BSWAP_32BIG_C(0x20e5fce1U),
    BSWAP_32BIG_C(0x7c3d5ba0U), BSWAP_32BIG_C(0xd2dcedbdU), BSWAP_32BIG_C(0xa95c7e00U), BSWAP_32BIG_C(0xdea51b22U),
    BSWAP_32BIG_C(0x5cff2ce2U), BSWAP_32BIG_C(0x7861e523U), BSWAP_32BIG_C(0xbb035506U), BSWAP_32BIG_C(0xf39b017aU),
    BSWAP_32BIG_C(0x85ffef06U), BSWAP_32BIG_C(0x8c8c9328U), BSWAP_32BIG_C(0x5e19a690U), BSWAP_32BIG_C(0x679030dbU),
    BSWAP_32BIG_C(0x49c5ed6dU), BSWAP_32BIG_C(0x7aa56b79U), BSWAP_32BIG_C(0x808728b0U), BSWAP_32BIG_C(0x3d0dabf3U),
    BSWAP_32BIG_C(0xa9c12a00U), BSWAP_32BIG_C(0xd67ea366U), BSWAP_32BIG_C(0x528ad6a6U), BSWAP_32BIG_C(0x0a509f7fU),
    BSWAP_32BIG_C(0x01ad0d7cU), BSWAP_32BIG_C(0x9269f954U), BSWAP_32BIG_C(0xf9201d43U), BSWAP_32BIG_C(0xb2a7e5e4U),
    BSWAP_32BIG_C(0x93c9a417U), BSWAP_32BIG_C(0x7950d8f6U), BSWAP_32BIG_C(0x2794418aU), BSWAP_32BIG_C(0xa30be44bU),
    BSWAP_32BIG_C(0x13a96875U), BSWAP_32BIG_C(0xac0c45b3U), BSWAP_32BIG_C(0x689ed97dU), BSWAP_32BIG_C(0xeba560cdU),
    BSWAP_32BIG_C(0x9724a053U), BSWAP_32BIG_C(0xfd078f3dU), BSWAP_32BIG_C(0xe36865c0U), BSWAP_32BIG_C(0x590ad323U),
    BSWAP_32BIG_C(0xb808c41bU), BSWAP_32BIG_C(0x1af855adU), BSWAP_32BIG_C(0x7c409673U), BSWAP_32BIG_C(0x8ad48e1eU),
    BSWAP_32BIG_C(0x68a3678fU), BSWAP_32BIG_C(0xca2137a3U), BSWAP_32BIG_C(0x738d062cU), BSWAP_32BIG_C(0xd0ac4d97U),
    BSWAP_32BIG_C(0xbeacf4baU), BSWAP_32BIG_C(0xa61c18ecU), BSWAP_32BIG_C(0xf0362fe1U), BSWAP_32BIG_C(0x18a61560U),
    BSWAP_32BIG_C(0x40bc68afU), BSWAP_32BIG_C(0x5c2a7a03U), BSWAP_32BIG_C(0x2e183427U), BSWAP_32BIG_C(0x36eff8b9U),
    BSWAP_32BIG_C(0xa5960827U), BSWAP_32BIG_C(0x9606d41bU), BSWAP_32BIG_C(0x4afb9a29U), BSWAP_32BIG_C(0x20b13d24U),
    BSWAP_32BIG_C(0xbb41fd40U), BSWAP_32BIG_C(0x03482936U), BSWAP_32BIG_C(0x3e6e7557U), BSWAP_32BIG_C(0x0868bbc9U),
    BSWAP_32BIG_C(0x1c886e71U), BSWAP_32BIG_C(0x39ecdb78U), BSWAP_32BIG_C(0xc6b983b6U), BSWAP_32BIG_C(0x560a12e3U),
    BSWAP_32BIG_C(0xeca618ccU), BSWAP_32BIG_C(0x79ca754dU), BSWAP_32BIG_C(0x1a5b2c93U), BSWAP_32BIG_C(0x9c616568U),
    BSWAP_32BIG_C(0xe8fb45a9U), BSWAP_32BIG_C(0x5e59a88aU), BSWAP_32BIG_C(0xe96c246aU), BSWAP_32BIG_C(0xd65aa43aU),
    BSWAP_32BIG_C(0x8c1a5b55U), BSWAP_32BIG_C(0xbc379280U), BSWAP_32BIG_C(0xb9a30278U), BSWAP_32BIG_C(0xef0dba99U),
};

uint32_t const DomainParam_NIST_Ed25519[] =
{
    BSWAP_32BIG_C(0x7e954da9U), BSWAP_32BIG_C(0xa5a8035eU), BSWAP_32BIG_C(0x292f36b3U), BSWAP_32BIG_C(0xd86e2375U),
    BSWAP_32BIG_C(0x7d7d7f8eU), BSWAP_32BIG_C(0x63969f29U), BSWAP_32BIG_C(0x44419852U), BSWAP_32BIG_C(0x3530be90U),
    BSWAP_32BIG_C(0x4e22ac6aU), BSWAP_32BIG_C(0xc4ffd911U), BSWAP_32BIG_C(0x1c93b71eU), BSWAP_32BIG_C(0x626a2d7aU),
    BSWAP_32BIG_C(0xdb5f29b6U), BSWAP_32BIG_C(0xa0ba4edaU), BSWAP_32BIG_C(0x26f76cccU), BSWAP_32BIG_C(0xbd8a5732U),
    BSWAP_32BIG_C(0x892d6d53U), BSWAP_32BIG_C(0x730e15e5U), BSWAP_32BIG_C(0x9fb7daacU), BSWAP_32BIG_C(0x1b8837a6U),
    BSWAP_32BIG_C(0x3da0e439U), BSWAP_32BIG_C(0xda7d1fd9U), BSWAP_32BIG_C(0xc576bf1fU), BSWAP_32BIG_C(0x01e2e938U),
    BSWAP_32BIG_C(0xb25a5ce1U), BSWAP_32BIG_C(0xe21f906bU), BSWAP_32BIG_C(0xe12c5bdfU), BSWAP_32BIG_C(0xa8c47883U),
    BSWAP_32BIG_C(0x17ff1a8dU), BSWAP_32BIG_C(0xf5af50c1U), BSWAP_32BIG_C(0x638fbc6eU), BSWAP_32BIG_C(0xaa75bec2U),
    BSWAP_32BIG_C(0x32b84b8fU), BSWAP_32BIG_C(0xec076e46U), BSWAP_32BIG_C(0xb55dd881U), BSWAP_32BIG_C(0x1a9a8ba6U),
    BSWAP_32BIG_C(0x3a71f8e0U), BSWAP_32BIG_C(0xb374a5ecU), BSWAP_32BIG_C(0x5ff7db04U), BSWAP_32BIG_C(0xd0752e14U),
    BSWAP_32BIG_C(0x66e95c0dU), BSWAP_32BIG_C(0x833c6a12U), BSWAP_32BIG_C(0xd2b953d5U), BSWAP_32BIG_C(0x5e5651fbU),
    BSWAP_32BIG_C(0xf9439fa0U), BSWAP_32BIG_C(0x253b7ad6U), BSWAP_32BIG_C(0x398b9033U), BSWAP_32BIG_C(0x1fa058b9U),
    BSWAP_32BIG_C(0x05b0e08bU), BSWAP_32BIG_C(0x3f81f154U), BSWAP_32BIG_C(0xb4f6aa56U), BSWAP_32BIG_C(0x8d9c165dU),
    BSWAP_32BIG_C(0xead6c0eaU), BSWAP_32BIG_C(0x671b37ebU), BSWAP_32BIG_C(0x96cce9a1U), BSWAP_32BIG_C(0x4d645c6fU),
    BSWAP_32BIG_C(0xdfdbec79U), BSWAP_32BIG_C(0x29a1132eU), BSWAP_32BIG_C(0x7f17293bU), BSWAP_32BIG_C(0x95f3eeb6U),
    BSWAP_32BIG_C(0x52f48b56U), BSWAP_32BIG_C(0x69f1c841U), BSWAP_32BIG_C(0xccc8bf93U), BSWAP_32BIG_C(0x7f64543dU),
    BSWAP_32BIG_C(0xe82eea75U), BSWAP_32BIG_C(0x7e38faa3U), BSWAP_32BIG_C(0xad2ad1b5U), BSWAP_32BIG_C(0x17c1ed15U),
    BSWAP_32BIG_C(0xd4f861beU), BSWAP_32BIG_C(0xfb198636U), BSWAP_32BIG_C(0x05a90426U), BSWAP_32BIG_C(0x6c87647cU),
    BSWAP_32BIG_C(0x83b9b954U), BSWAP_32BIG_C(0x471059e9U), BSWAP_32BIG_C(0x98c34a13U), BSWAP_32BIG_C(0x8a117171U),
};

uint32_t const DomainParam_Brainpool_256r1[] = 
{
    BSWAP_32BIG_C(0xedfe3eaaU), BSWAP_32BIG_C(0x60cca332U), BSWAP_32BIG_C(0xc40ba65cU), BSWAP_32BIG_C(0x5b15ca42U),
    BSWAP_32BIG_C(0x08394b93U), BSWAP_32BIG_C(0x8779b3ecU), BSWAP_32BIG_C(0xfc081496U), BSWAP_32BIG_C(0x60434e99U),
    BSWAP_32BIG_C(0x4552fa44U), BSWAP_32BIG_C(0x66e57d63U), BSWAP_32BIG_C(0x015876d6U), BSWAP_32BIG_C(0x3757ad69U),
    BSWAP_32BIG_C(0x3261babdU), BSWAP_32BIG_C(0xd8ecfa94U), BSWAP_32BIG_C(0x17f85400U), BSWAP_32BIG_C(0x234f150eU),
    BSWAP_32BIG_C(0x5ea911e5U), BSWAP_32BIG_C(0x3f2ceb6cU), BSWAP_32BIG_C(0x6c2897d7U), BSWAP_32BIG_C(0x3bf2d8ecU),
    BSWAP_32BIG_C(0x18514cf3U), BSWAP_32BIG_C(0x44c9e543U), BSWAP_32BIG_C(0x8997fd59U), BSWAP_32BIG_C(0x8b40bc41U),
    BSWAP_32BIG_C(0xb17782eeU), BSWAP_32BIG_C(0xef4fd5c8U), BSWAP_32BIG_C(0x97540d29U), BSWAP_32BIG_C(0xc161589bU),
    BSWAP_32BIG_C(0x74f8160cU), BSWAP_32BIG_C(0x8d51c2f3U), BSWAP_32BIG_C(0x81c87ef2U), BSWAP_32BIG_C(0x0a41096cU),
    BSWAP_32BIG_C(0xad8d3cb0U), BSWAP_32BIG_C(0xadf0c76cU), BSWAP_32BIG_C(0xa40e5103U), BSWAP_32BIG_C(0xb2c5af11U),
    BSWAP_32BIG_C(0x9365af9aU), BSWAP_32BIG_C(0xebfe4c00U), BSWAP_32BIG_C(0xb14dfb71U), BSWAP_32BIG_C(0x477c31cdU),
    BSWAP_32BIG_C(0x0e11171eU), BSWAP_32BIG_C(0x9d536a58U), BSWAP_32BIG_C(0x152c5afdU), BSWAP_32BIG_C(0xe29fcc17U),
    BSWAP_32BIG_C(0x437a6cf9U), BSWAP_32BIG_C(0x71b57bdeU), BSWAP_32BIG_C(0x425e0698U), BSWAP_32BIG_C(0x6e12427aU),
    BSWAP_32BIG_C(0xc73460b9U), BSWAP_32BIG_C(0x42b6bfc5U), BSWAP_32BIG_C(0x53f33de0U), BSWAP_32BIG_C(0x0537d279U),
    BSWAP_32BIG_C(0x1d4a9a68U), BSWAP_32BIG_C(0x35c39c6bU), BSWAP_32BIG_C(0x70fc32e7U), BSWAP_32BIG_C(0x7b81fb79U),
    BSWAP_32BIG_C(0x144ef689U), BSWAP_32BIG_C(0x6fe757c8U), BSWAP_32BIG_C(0x8b1ce932U), BSWAP_32BIG_C(0x39e47c27U),
    BSWAP_32BIG_C(0xb3880125U), BSWAP_32BIG_C(0xa5be4d73U), BSWAP_32BIG_C(0xf7fabd06U), BSWAP_32BIG_C(0x0f58ca9aU),
    BSWAP_32BIG_C(0x0af7048dU), BSWAP_32BIG_C(0x6f6fa32dU), BSWAP_32BIG_C(0xfea15cafU), BSWAP_32BIG_C(0x4dac41f8U),
    BSWAP_32BIG_C(0xadd1269fU), BSWAP_32BIG_C(0x253e68a0U), BSWAP_32BIG_C(0xf34017cfU), BSWAP_32BIG_C(0x31111c43U),
};

uint32_t const DomainParam_Brainpool_384r1[] = 
{
    BSWAP_32BIG_C(0x5af47deaU), BSWAP_32BIG_C(0x63f28e9dU), BSWAP_32BIG_C(0x2d8fe31fU), BSWAP_32BIG_C(0x0bce7626U),
    BSWAP_32BIG_C(0x4f351c2eU), BSWAP_32BIG_C(0x8082d50bU), BSWAP_32BIG_C(0xb4783637U), BSWAP_32BIG_C(0xb9d1d672U),
    BSWAP_32BIG_C(0x2f6a9b6fU), BSWAP_32BIG_C(0x7b0a0847U), BSWAP_32BIG_C(0x9e8e9e53U), BSWAP_32BIG_C(0xd1fd8c14U),
    BSWAP_32BIG_C(0xf844d19cU), BSWAP_32BIG_C(0x22e2ed6cU), BSWAP_32BIG_C(0x40fcdc97U), BSWAP_32BIG_C(0x451a8443U),
    BSWAP_32BIG_C(0x0c8c4cf0U), BSWAP_32BIG_C(0x7a43c7faU), BSWAP_32BIG_C(0xc749bf0bU), BSWAP_32BIG_C(0x49bb87ebU),
    BSWAP_32BIG_C(0x1f15d9fbU), BSWAP_32BIG_C(0x6cb4945cU), BSWAP_32BIG_C(0x432cd564U), BSWAP_32BIG_C(0xf00f6f79U),
    BSWAP_32BIG_C(0xcf87a42cU), BSWAP_32BIG_C(0x531ccc07U), BSWAP_32BIG_C(0xe9cb004cU), BSWAP_32BIG_C(0x49d0798dU),
    BSWAP_32BIG_C(0xb1669415U), BSWAP_32BIG_C(0xad54e498U), BSWAP_32BIG_C(0x6c648847U), BSWAP_32BIG_C(0x07c6a03aU),
    BSWAP_32BIG_C(0xe5cb3975U), BSWAP_32BIG_C(0x9c1ae0ebU), BSWAP_32BIG_C(0xfcca95e3U), BSWAP_32BIG_C(0xf319072dU),
    BSWAP_32BIG_C(0x73951576U), BSWAP_32BIG_C(0x74721ee4U), BSWAP_32BIG_C(0x418fa196U), BSWAP_32BIG_C(0x02172560U),
    BSWAP_32BIG_C(0x0e58541cU), BSWAP_32BIG_C(0xad5c599aU), BSWAP_32BIG_C(0xa3e311cfU), BSWAP_32BIG_C(0xdad89377U),
    BSWAP_32BIG_C(0x9f321ae9U), BSWAP_32BIG_C(0x37fdbb5bU), BSWAP_32BIG_C(0x7c4b86c4U), BSWAP_32BIG_C(0x4c3686ebU),
    BSWAP_32BIG_C(0x9ef342e6U), BSWAP_32BIG_C(0x5889a02bU), BSWAP_32BIG_C(0x4184b209U), BSWAP_32BIG_C(0xff7277e0U),
    BSWAP_32BIG_C(0x44239909U), BSWAP_32BIG_C(0xbb4381e6U), BSWAP_32BIG_C(0x97a43546U), BSWAP_32BIG_C(0xa538ab8eU),
    BSWAP_32BIG_C(0xf8b9182cU), BSWAP_32BIG_C(0xdbe5e2a7U), BSWAP_32BIG_C(0x9d51fbb0U), BSWAP_32BIG_C(0x04262d8dU),
    BSWAP_32BIG_C(0xf1e65d86U), BSWAP_32BIG_C(0x58139700U), BSWAP_32BIG_C(0x01a721a5U), BSWAP_32BIG_C(0x8ae61429U),
    BSWAP_32BIG_C(0xf0e8dff6U), BSWAP_32BIG_C(0xd263d9adU), BSWAP_32BIG_C(0x08defbf6U), BSWAP_32BIG_C(0xb59c41c8U),
    BSWAP_32BIG_C(0x9ff5666fU), BSWAP_32BIG_C(0x496f64fdU), BSWAP_32BIG_C(0x1892bd1fU), BSWAP_32BIG_C(0x0f442820U),
    BSWAP_32BIG_C(0x273fca54U), BSWAP_32BIG_C(0xa7c2fdb6U), BSWAP_32BIG_C(0x231a20a4U), BSWAP_32BIG_C(0x0aa0b20fU),
    BSWAP_32BIG_C(0xa8418d13U), BSWAP_32BIG_C(0x8a5b7ae0U), BSWAP_32BIG_C(0x332ae50dU), BSWAP_32BIG_C(0x41af6d76U),
    BSWAP_32BIG_C(0xe0dbaa9cU), BSWAP_32BIG_C(0x0aaf44bcU), BSWAP_32BIG_C(0xfbb63eafU), BSWAP_32BIG_C(0xa45aad20U),
    BSWAP_32BIG_C(0x0f247da5U), BSWAP_32BIG_C(0x16ff80abU), BSWAP_32BIG_C(0x6928e857U), BSWAP_32BIG_C(0x77931c49U),
    BSWAP_32BIG_C(0x0a513afeU), BSWAP_32BIG_C(0xdf82aef2U), BSWAP_32BIG_C(0xc7ec95c4U), BSWAP_32BIG_C(0x5932731aU),
    BSWAP_32BIG_C(0xc3ff6021U), BSWAP_32BIG_C(0x5fe1b0f4U), BSWAP_32BIG_C(0xdb67f487U), BSWAP_32BIG_C(0x72531f68U),
    BSWAP_32BIG_C(0xc3737a8dU), BSWAP_32BIG_C(0xa0273e95U), BSWAP_32BIG_C(0xae241acbU), BSWAP_32BIG_C(0xa0ce40ddU),
    BSWAP_32BIG_C(0x6e6bb314U), BSWAP_32BIG_C(0x68582a5dU), BSWAP_32BIG_C(0xf1653edeU), BSWAP_32BIG_C(0xc62e358fU),
};

uint32_t const DomainParam_Brainpool_512r1[] = 
{
    BSWAP_32BIG_C(0xf1cb294dU), BSWAP_32BIG_C(0x1cf2464eU), BSWAP_32BIG_C(0x528afd58U), BSWAP_32BIG_C(0x14d85df8U),
    BSWAP_32BIG_C(0x615d0538U), BSWAP_32BIG_C(0x6db75452U), BSWAP_32BIG_C(0xdfe9cbb8U), BSWAP_32BIG_C(0x64b7bb7aU),
    BSWAP_32BIG_C(0x51e3f2ecU), BSWAP_32BIG_C(0x5db1eee5U), BSWAP_32BIG_C(0xf9bca9d3U), BSWAP_32BIG_C(0x174a369eU),
    BSWAP_32BIG_C(0x3f1bcf73U), BSWAP_32BIG_C(0x63675a89U), BSWAP_32BIG_C(0x22ea0c31U), BSWAP_32BIG_C(0x5a0160f7U),
    BSWAP_32BIG_C(0x3e44084cU), BSWAP_32BIG_C(0x1aebae8dU), BSWAP_32BIG_C(0x81df8311U), BSWAP_32BIG_C(0x1821be87U),
    BSWAP_32BIG_C(0x724afba7U), BSWAP_32BIG_C(0xb42583e2U), BSWAP_32BIG_C(0x09e263a4U), BSWAP_32BIG_C(0x6a561a71U),
    BSWAP_32BIG_C(0xdbbfc13aU), BSWAP_32BIG_C(0x0b6fbfd0U), BSWAP_32BIG_C(0x41ecd1a6U), BSWAP_32BIG_C(0xed04a5d1U),
    BSWAP_32BIG_C(0x5029bc29U), BSWAP_32BIG_C(0xe847c63aU), BSWAP_32BIG_C(0x8ab05f1fU), BSWAP_32BIG_C(0xec1fd565U),
    BSWAP_32BIG_C(0xcf3fed7eU), BSWAP_32BIG_C(0x435d74baU), BSWAP_32BIG_C(0x09581bc4U), BSWAP_32BIG_C(0x06fae2bdU),
    BSWAP_32BIG_C(0x497b903aU), BSWAP_32BIG_C(0x20d53be2U), BSWAP_32BIG_C(0xfbd7b193U), BSWAP_32BIG_C(0x523f76c8U),
    BSWAP_32BIG_C(0x6c60616fU), BSWAP_32BIG_C(0x784206bbU), BSWAP_32BIG_C(0x522c144eU), BSWAP_32BIG_C(0x23490627U),
    BSWAP_32BIG_C(0xf093a77cU), BSWAP_32BIG_C(0x5ce92c6dU), BSWAP_32BIG_C(0xc8a461e6U), BSWAP_32BIG_C(0x064e0521U),
    BSWAP_32BIG_C(0x7fd8077eU), BSWAP_32BIG_C(0x1b29332dU), BSWAP_32BIG_C(0x8bdb778eU), BSWAP_32BIG_C(0x71031464U),
    BSWAP_32BIG_C(0x644d626eU), BSWAP_32BIG_C(0x5be163fcU), BSWAP_32BIG_C(0x93eb8853U), BSWAP_32BIG_C(0xa3f5743bU),
    BSWAP_32BIG_C(0xd1255f8dU), BSWAP_32BIG_C(0x943461ddU), BSWAP_32BIG_C(0x0b98989aU), BSWAP_32BIG_C(0x762a3af2U),
    BSWAP_32BIG_C(0x1ce4b678U), BSWAP_32BIG_C(0x51bb714bU), BSWAP_32BIG_C(0x724d46b5U), BSWAP_32BIG_C(0x80ffe8ceU),
    BSWAP_32BIG_C(0xb881f838U), BSWAP_32BIG_C(0xc7741d40U), BSWAP_32BIG_C(0x4c2b7842U), BSWAP_32BIG_C(0xa7f02d54U),
    BSWAP_32BIG_C(0x3bc74902U), BSWAP_32BIG_C(0x151306d9U), BSWAP_32BIG_C(0x29908baeU), BSWAP_32BIG_C(0xc8c0fc70U),
    BSWAP_32BIG_C(0x012edd8aU), BSWAP_32BIG_C(0x3a427b1cU), BSWAP_32BIG_C(0x044416c7U), BSWAP_32BIG_C(0x94662783U),
    BSWAP_32BIG_C(0xfe747d98U), BSWAP_32BIG_C(0x167632c3U), BSWAP_32BIG_C(0xa268c188U), BSWAP_32BIG_C(0x86b6a6e5U),
    BSWAP_32BIG_C(0x2beee4a4U), BSWAP_32BIG_C(0xa8f4a8f0U), BSWAP_32BIG_C(0xf8ee7b3eU), BSWAP_32BIG_C(0x3c6eb6a9U),
    BSWAP_32BIG_C(0xe5f5605cU), BSWAP_32BIG_C(0x624aed62U), BSWAP_32BIG_C(0x1dcd888aU), BSWAP_32BIG_C(0xe16555dfU),
    BSWAP_32BIG_C(0xefe68310U), BSWAP_32BIG_C(0xe8978c8fU), BSWAP_32BIG_C(0xaa088d92U), BSWAP_32BIG_C(0x78cb8467U),
    BSWAP_32BIG_C(0xafd20ac1U), BSWAP_32BIG_C(0xa0909bc2U), BSWAP_32BIG_C(0xa72f5c8dU), BSWAP_32BIG_C(0xec517a8fU),
    BSWAP_32BIG_C(0x68b454eeU), BSWAP_32BIG_C(0x4c8b4bfcU), BSWAP_32BIG_C(0xb25d217cU), BSWAP_32BIG_C(0x267ebc1dU),
    BSWAP_32BIG_C(0xd64b5feaU), BSWAP_32BIG_C(0xd49c40e7U), BSWAP_32BIG_C(0x0805c3e6U), BSWAP_32BIG_C(0x13fb307bU),
    BSWAP_32BIG_C(0x236769c9U), BSWAP_32BIG_C(0x43d1c632U), BSWAP_32BIG_C(0x7bbc60beU), BSWAP_32BIG_C(0x344e74bfU),
    BSWAP_32BIG_C(0x7a3c8a43U), BSWAP_32BIG_C(0xcb212038U), BSWAP_32BIG_C(0x57d450ebU), BSWAP_32BIG_C(0xaa23e1ceU),
    BSWAP_32BIG_C(0xa2e61d0eU), BSWAP_32BIG_C(0x125f0312U), BSWAP_32BIG_C(0x4444b76fU), BSWAP_32BIG_C(0xd4eff35cU),
    BSWAP_32BIG_C(0xec26cee7U), BSWAP_32BIG_C(0x92673fa8U), BSWAP_32BIG_C(0x4ca68a96U), BSWAP_32BIG_C(0xbf7b9fa2U),
    BSWAP_32BIG_C(0xaa259ea7U), BSWAP_32BIG_C(0xe7c08bf8U), BSWAP_32BIG_C(0x99a17e25U), BSWAP_32BIG_C(0x2bd426f4U),
    BSWAP_32BIG_C(0xd0af6e60U), BSWAP_32BIG_C(0xc0753e74U), BSWAP_32BIG_C(0x12ae5b37U), BSWAP_32BIG_C(0xbd95728eU),
    BSWAP_32BIG_C(0x98090a9fU), BSWAP_32BIG_C(0x04fc6f93U), BSWAP_32BIG_C(0x8271d0f9U), BSWAP_32BIG_C(0x3d018521U),
    BSWAP_32BIG_C(0xd0ae694dU), BSWAP_32BIG_C(0xc9d098a6U), BSWAP_32BIG_C(0x5025087bU), BSWAP_32BIG_C(0xc522683bU),
};

uint32_t const DomainParam_Koblitz_secp256k1[] =
{
    BSWAP_32BIG_C(0x8ab7bc7eU), BSWAP_32BIG_C(0x60c4601aU), BSWAP_32BIG_C(0x1090109eU), BSWAP_32BIG_C(0x10816c9eU),
    BSWAP_32BIG_C(0xf889c01cU), BSWAP_32BIG_C(0xac9e6736U), BSWAP_32BIG_C(0xdf536181U), BSWAP_32BIG_C(0x9a5cf89eU),
    BSWAP_32BIG_C(0xc1063010U), BSWAP_32BIG_C(0x8e9c99b6U), BSWAP_32BIG_C(0xdf01bac0U), BSWAP_32BIG_C(0x2787ae77U),
    BSWAP_32BIG_C(0x911ef89bU), BSWAP_32BIG_C(0xaba8d56dU), BSWAP_32BIG_C(0xe3d76f91U), BSWAP_32BIG_C(0x4a25c671U),
    BSWAP_32BIG_C(0x70d6d2cdU), BSWAP_32BIG_C(0x4859e1e1U), BSWAP_32BIG_C(0x132f9128U), BSWAP_32BIG_C(0x83d56b42U),
    BSWAP_32BIG_C(0x6a4f5a16U), BSWAP_32BIG_C(0x91cc0fb6U), BSWAP_32BIG_C(0x54fd12adU), BSWAP_32BIG_C(0x4878c7e7U),
    BSWAP_32BIG_C(0x7c727a8fU), BSWAP_32BIG_C(0xc174a90eU), BSWAP_32BIG_C(0x2aaae7d1U), BSWAP_32BIG_C(0x68dd6db6U),
    BSWAP_32BIG_C(0xbdbcfd2dU), BSWAP_32BIG_C(0xed095eecU), BSWAP_32BIG_C(0x5423f3feU), BSWAP_32BIG_C(0xaa49fb61U),
    BSWAP_32BIG_C(0x98d6fc5dU), BSWAP_32BIG_C(0xed01f5c8U), BSWAP_32BIG_C(0x48b7e964U), BSWAP_32BIG_C(0x593d27d1U),
    BSWAP_32BIG_C(0xada4442dU), BSWAP_32BIG_C(0xe7ba1636U), BSWAP_32BIG_C(0xe334019dU), BSWAP_32BIG_C(0x45b442e6U),
    BSWAP_32BIG_C(0x7761876eU), BSWAP_32BIG_C(0x8923a8b9U), BSWAP_32BIG_C(0x9f9f242bU), BSWAP_32BIG_C(0xefda4b9dU),
    BSWAP_32BIG_C(0x1dc4a72eU), BSWAP_32BIG_C(0xff1e9c9dU), BSWAP_32BIG_C(0x7d94a0e6U), BSWAP_32BIG_C(0xc68919b3U),
    BSWAP_32BIG_C(0x1f090536U), BSWAP_32BIG_C(0xffeae12bU), BSWAP_32BIG_C(0x7217f173U), BSWAP_32BIG_C(0x030c960fU),
    BSWAP_32BIG_C(0x8703845eU), BSWAP_32BIG_C(0x5f43e097U), BSWAP_32BIG_C(0x494d2673U), BSWAP_32BIG_C(0x492109d2U),
    BSWAP_32BIG_C(0x25e53c51U), BSWAP_32BIG_C(0x6f2aaca2U), BSWAP_32BIG_C(0xb9923498U), BSWAP_32BIG_C(0x36c6f390U),
    BSWAP_32BIG_C(0xc5250c3cU), BSWAP_32BIG_C(0xa486353aU), BSWAP_32BIG_C(0x2d23d677U), BSWAP_32BIG_C(0x18bef955U),
    BSWAP_32BIG_C(0x2b33ac23U), BSWAP_32BIG_C(0x91ac4a3bU), BSWAP_32BIG_C(0x4ed4a36bU), BSWAP_32BIG_C(0xe273eac4U),
    BSWAP_32BIG_C(0xb480b510U), BSWAP_32BIG_C(0x5978645cU), BSWAP_32BIG_C(0x76491482U), BSWAP_32BIG_C(0xba9bf7afU),
};

