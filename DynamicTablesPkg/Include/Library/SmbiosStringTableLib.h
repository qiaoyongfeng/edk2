/** @file
  SMBIOS String Table Helper library.

  Copyright (c) 2022, Arm Limited. All rights reserved.<BR>
  SPDX-License-Identifier: BSD-2-Clause-Patent
**/

#ifndef SMBIOS_STRING_TABLE_H_
#define SMBIOS_STRING_TABLE_H_

/** A structure representing a string in the string table.
*/
typedef struct StringElement {
  /// Length of the string (does not include the NULL termination)
  UINTN          StringLen;

  /// Reference to the string
  CONST CHAR8    *String;
} STRING_ELEMENT;

/** A structure representing a string table.
*/
typedef struct StringTable {
  /// Count of strings in the table
  UINT8             StrCount;

  /// Total length of all strings in the table (does not include
  // the NULL termination for each string)
  UINTN             TotalStrLen;

  /// Maximum string count
  UINT8             MaxStringElements;

  /// Pointer to the string table elements
  STRING_ELEMENT    *Elements;
} STRING_TABLE;

/** Add a string to the string table

  @param [IN]   StrTable  Pointer to the string table
  @param [IN]   Str       Pointer to the string
  @param [OUT]  StrRef    Optional pointer to retrieve the string field
                          reference of the string in the string table

  @return EFI_SUCCESS            Success
  @return EFI_INVALID_PARAMETER  Invalid string table pointer
  @return EFI_BUFFER_TOO_SMALL   Insufficient space to add string
**/
EFI_STATUS
EFIAPI
StringTableAddString (
  IN        STRING_TABLE *CONST  StrTable,
  IN  CONST CHAR8                *Str,
  OUT       UINT8                *StrRef      OPTIONAL
  );

/** Returns the total size required to publish the strings to the SMBIOS
    string area.

  @param [IN] StrTable              Pointer to the string table

  @return Total size required to publish the strings in the SMBIOS string area.
**/
UINTN
EFIAPI
StringTableGetStringSetSize (
  IN  STRING_TABLE *CONST  StrTable
  );

/** Iterate through the string table and publish the strings in the SMBIOS
    string area.

  @param [IN] StrTable              Pointer to the string table
  @param [IN] SmbiosStringAreaStart Start address of the SMBIOS string area.
  @param [IN] SmbiosStringAreaSize  Size of the SMBIOS string area.

  @return EFI_SUCCESS            Success
  @return EFI_INVALID_PARAMETER  Invalid string table pointer
  @return EFI_BUFFER_TOO_SMALL   Insufficient space to publish strings
**/
EFI_STATUS
EFIAPI
StringTablePublishStringSet (
  IN        STRING_TABLE  *CONST  StrTable,
  IN        CHAR8         *CONST  SmbiosStringAreaStart,
  IN  CONST UINTN                 SmbiosStringAreaSize
  );

/** Initialise the string table and allocate memory for the string elements.

  @param [IN] StrTable           Pointer to the string table
  @param [IN] MaxStringElements  Maximum number of strings that the string
                                 table can hold.

  @return EFI_SUCCESS            Success
  @return EFI_INVALID_PARAMETER  Invalid string table pointer
  @return EFI_OUT_OF_RESOURCES   Failed to allocate memory for string elements
**/
EFI_STATUS
EFIAPI
StringTableInitialize (
  IN STRING_TABLE *CONST  StrTable,
  IN UINTN                MaxStringElements
  );

/** Free memory allocated for the string elements in the string table.

  @param [IN] StrTable           Pointer to the string table

  @return EFI_SUCCESS            Success
  @return EFI_INVALID_PARAMETER  Invalid string table pointer or string elements
**/
EFI_STATUS
EFIAPI
StringTableFree (
  IN STRING_TABLE *CONST  StrTable
  );

/** STRING_TABLE_ADD_STRING macro is the wrapper over StringTableAddString().
    It adds a string to the string table, and if the string is NULL or empty,
    add "Unknown" string instead.

  @param [IN]   StrTable  Pointer to the string table
  @param [IN]   Str       Pointer to the string
  @param [OUT]  StrRef    The string field reference of the added string
**/
#define STRING_TABLE_ADD_STRING(StrTable, String, StringRef)              \
  StringRef = 0;                                                          \
  if ((String != NULL) && (String[0] != '\0')) {                          \
    Status = StringTableAddString (&StrTable, String, &StringRef);        \
  } else {                                                                \
    Status = StringTableAddString (&StrTable, "Unknown", &StringRef);     \
  }                                                                       \
  if (EFI_ERROR (Status)) {                                               \
    DEBUG ((DEBUG_ERROR, "Failed to add "#String" string %r\n", Status)); \
  }                                                                       \

#endif // SMBIOS_STRING_TABLE_H_
