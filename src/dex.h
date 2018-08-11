/*

   DEX Repair
   -----------------------------------------

   Anestis Bechtsoudis <anestis@census-labs.com>
   Copyright 2015-2016 by CENSUS S.A. All Rights Reserved.

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

*/

#ifndef _DEX_H_
#define _DEX_H_

#include <zlib.h>
#include "sha1.h"

typedef uint8_t   u1;
typedef uint16_t  u2;
typedef uint32_t  u4;
typedef uint64_t  u8;
typedef int8_t    s1;
typedef int16_t   s2;
typedef int32_t   s4;
typedef int64_t   s8;

#define kNumDexVersions 4
#define kNumCDexVersions 1
#define kDexVersionLen 4
#define kSHA1Len SHA1HashSize

static const uint8_t kDexMagic[] = { 'd', 'e', 'x', '\n' };
static const uint8_t kDexMagicVersions[kNumDexVersions][kDexVersionLen] = {
  { '0', '3', '5', '\0' },
  // Dex version 036 skipped
  { '0', '3', '7', '\0' },
  // Dex version 038: Android "O".
  { '0', '3', '8', '\0' },
  // Dex verion 039: Beyond Android "O".
  { '0', '3', '9', '\0' },
};

static const uint8_t kCDexMagic[] = { 'c', 'd', 'e', 'x' };
static const uint8_t kCDexMagicVersions[kNumCDexVersions][kDexVersionLen] = {
  // Android P
  { '0', '0', '1', '\0' },
};

typedef struct __attribute__((packed)) {
    char dex[4];
    char ver[4];
} dexMagic;

typedef struct __attribute__((packed)) {
    dexMagic magic;
    u4 checksum;
    unsigned char signature[kSHA1Len];
    u4 fileSize;
    u4 headerSize;
    u4 endianTag;
    u4 linkSize;
    u4 linkOff;
    u4 mapOff;
    u4 stringIdsSize;
    u4 stringIdsOff;
    u4 typeIdsSize;
    u4 typeIdsOff;
    u4 protoIdsSize;
    u4 protoIdsOff;
    u4 fieldIdsSize;
    u4 fieldIdsOff;
    u4 methodIdsSize;
    u4 methodIdsOff;
    u4 classDefsSize;
    u4 classDefsOff;
    u4 dataSize;
    u4 dataOff;
} dexHeader;

typedef struct __attribute__((packed)) {
    dexMagic magic;
    u4  dexOff;
    u4  dexSize;
    u4  depsOff;
    u4  depsSize;
    u4  optOff;
    u4  optSize;
    u4  flags;
    u4  checksum;
} odexHeader;

typedef struct __attribute__((packed)) {
    u4 stringDataOff;
} dexStringId;

typedef struct __attribute__((packed)) {
    u4 descriptorIdx;
} dexTypeId;

typedef struct __attribute__((packed)) {
    u2  classIdx;
    u2  typeIdx;
    u4  nameIdx;
} dexFieldId;

typedef struct __attribute__((packed)) {
    u2  classIdx;
    u2  protoIdx;
    u4  nameIdx;
} dexMethodId;

typedef struct __attribute__((packed)) {
    u4  shortyIdx;
    u4  returnTypeIdx;
    u4  parametersOff;
} dexProtoId;

typedef struct __attribute__((packed)) {
    u4  classIdx;
    u4  accessFlags;
    u4  superclassOdx;
    u4  interfacesOff;
    u4  sourceFileIdx;
    u4  annotationsOff;
    u4  classDataOff;
    u4  staticValuesOff;
} dexClassDef;

typedef struct __attribute__((packed)) {
    u2  typeIdx;
} dexTypeItem;

typedef struct __attribute__((packed)) {
    u4  size;
    dexTypeItem list[1];
} dexTypeList;

typedef struct __attribute__((packed)) {
    u2 type;
    u2 unused;
    u4 size;
    u4 offset;
} dexMapItem;

typedef struct __attribute__((packed)) {
    u4  size;
    dexMapItem list[1];
} dexMapList;

typedef struct __attribute__((packed)) {
    u2  registersSize;
    u2  insSize;
    u2  outsSize;
    u2  tries_size;
    u4  debug_info_off;
    u4  insns_size;
    u2  insns[1];
    /* followed by optional u2 padding */
    /* followed by try_item[triesSize] */
    /* followed by uleb128 handlersSize */
    /* followed by catch_handler_item[handlersSize] */
} dexCode;

typedef struct __attribute__((packed)) {
    u4 start_addr_;
    u2 insn_count_;
    u2 handler_off_;
} dexTryItem;

typedef struct __attribute__((packed)) {
    u1  bleargh;
} dexLinkData;

typedef struct __attribute__((packed)) {
    int     size;
    int     numEntries;
    struct {
        u4      classDescriptorHash;
        int     classDescriptorOff;
        int     classDefOff;
    } table[1];
} dexClassLookup;

typedef struct __attribute__((packed)) {
    odexHeader     *pOdexHeader;
    dexHeader      *pDexHeader;
    dexStringId    *pDexStringIds;
    dexTypeId      *pDexTypeIds;
    dexFieldId     *pDexFieldIds;
    dexMethodId    *pDexMethodIds;
    dexProtoId     *pDexProtoIds;
    dexClassDef    *pDexClassDefs;
    dexLinkData    *pDexLinkData;
    dexClassLookup *pclassLookup;
    void           *pregisterMapPool;
    u1             *baseAddr;
    int            overhead;
    //void*          auxData;
} dexFile;

typedef struct __attribute__((packed)) {
    u4 staticFieldsSize;
    u4 instanceFieldsSize;
    u4 directMethodsSize;
    u4 virtualMethodsSize;
} dexClassDataHeader;

typedef struct __attribute__((packed)) {
    u4 methodIdx;
    u4 accessFlags;
    u4 codeOff;
} dexMethod;

typedef struct __attribute__((packed)) {
    u4 fieldIdx;
    u4 accessFlags;
} dexField;

typedef struct __attribute__((packed)) {
    dexClassDataHeader header;
    dexField           *staticFields;
    dexField           *instanceFields;
    dexMethod          *directMethods;
    dexMethod          *virtualMethods;
} dexClassData;

/*
 * Verify if valid DEX file magic number
 */
bool dex_isValidDexMagic(const dexHeader *pDexHeader)
{
  /* Validate magic number */
  if (memcmp(pDexHeader->magic.dex, kDexMagic, sizeof(kDexMagic)) != 0) {
    return false;
  }

  /* Validate magic version */
  const char *version = pDexHeader->magic.ver;
  for (uint32_t i = 0; i < kNumDexVersions; i++) {
    if (memcmp(version, kDexMagicVersions[i], kDexVersionLen) == 0) {
      LOGMSG(l_DEBUG, "DEX version '%s' detected", pDexHeader->magic.ver);
      return true;
    }
  }

  for (uint32_t i = 0; i < kNumCDexVersions; i++) {
    if (memcmp(version, kCDexMagicVersions[i], kDexVersionLen) == 0) {
      LOGMSG(l_DEBUG, "CDEX version '%s' detected", pDexHeader->magic.ver);
      return true;
    }
  }
  return false;
}

/*
 * Repair DEX file CRC
 */
void dex_repairDexCRC(uint8_t *buf, off_t fileSz)
{
    /* Repair DEX CRC */
    uint32_t adler_checksum = adler32(0L, Z_NULL, 0);
    const uint8_t non_sum = sizeof(dexMagic) + sizeof(uint32_t);
    const uint8_t *non_sum_ptr = (const uint8_t*)buf + non_sum;
    adler_checksum = adler32(adler_checksum, non_sum_ptr, fileSz - non_sum);
    memcpy(buf + sizeof(dexMagic), &adler_checksum, sizeof(uint32_t));
}

/*
 * Repair DEX file SHA-1
 */
bool dex_repairDexSHA1(uint8_t *buf, off_t fileSz)
{
    const uint8_t sha1_off = sizeof(dexMagic) + sizeof(uint32_t);
    const uint8_t non_sha1_off = sizeof(dexMagic) + sizeof(uint32_t) + (kSHA1Len * sizeof(char));
    const uint8_t *non_sha1_ptr = (const uint8_t*)buf + non_sha1_off;

    SHA1Context sha;
    if (SHA1Reset(&sha)) {
        return false;
    }

    if (SHA1Input(&sha, non_sha1_ptr, fileSz - non_sha1_off)) {
        return false;
    }

    if (SHA1Result(&sha, buf + sha1_off)) {
        return false;
    }

    return true;
}

#endif
