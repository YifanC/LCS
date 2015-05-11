/*!
  \file commands.c
  \brief this file implements the basic command execution routines.

*/

#include "commands.h"
#include <string.h>

EIB7_ERR CmdIF_InitializeCommand(EIB_Data *ed, int command, void **cmddata)
{
  if(ed == NULL || ed->cif == NULL)
    return EIB7_NotInitialized;
  
  return ed->cif->InitializeCommand(command, cmddata);
}

EIB7_ERR CmdIF_FreeCommand(EIB_Data *ed, void *cmddata)
{
  if(ed == NULL || ed->cif == NULL)
    return EIB7_NotInitialized;
  
  return ed->cif->FreeCommand(cmddata);
}

EIB7_ERR CmdIF_SetParameter(EIB_Data *ed, void *cmddata, int index, void *value, int size)
{
  if(ed == NULL || ed->cif == NULL)
    return EIB7_NotInitialized;
  
  return ed->cif->SetParameter(cmddata, index, value, size);
}

EIB7_ERR CmdIF_GetParameter(EIB_Data *ed, void *cmddata, int index, void *value, int size, int offset)
{
  if(ed == NULL || ed->cif == NULL)
    return EIB7_NotInitialized;
  
  return ed->cif->GetParameter(cmddata, index, value, size, offset);
}

EIB7_ERR CmdIF_ExecuteCommand(EIB_Data *ed, void *cmddata, int *resplen)
{
  if(ed == NULL || ed->cif == NULL)
    return EIB7_NotInitialized;
  
  return ed->cif->ExecuteCommand(ed->conn, ed->seqno++, cmddata, resplen);
}


/*! some internal definitions for EIB7_PositionDataField for auxiliary axis */
#define EIB7_PDF_AUXStatus       0x02000
#define EIB7_PDF_AUXPosition     0x04000
#define EIB7_PDF_AUXTimestamp    0x08000
#define EIB7_PDF_AUXRefPos       0x10000


/*! These arrays define the sizes of all available data fields in a position data structure */
EIB_DataFieldDescription EIB_DataFieldDefinitionGlobal[] =
{
   /* type, size, size_raw, offset, offset_raw, next */
   { EIB7_PDF_TriggerCounter,2, 2, 0, 0, NULL },         /* 0 */
   { EIB7_PDF_StatusWord,    2, 2, 0, 0, NULL },         /* 1 */
   { EIB7_PDF_PositionData,  8, 6, 0, 0, NULL },         /* 2 */
   { EIB7_PDF_Timestamp,     4, 4, 0, 0, NULL },         /* 3 */
   { EIB7_PDF_ReferencePos, 16,12, 0, 0, NULL },         /* 4 */
   { EIB7_PDF_DistCodedRef,  8, 6, 0, 0, NULL },         /* 5 */
   { EIB7_PDF_Analog,        4, 4, 0, 0, NULL },         /* 6 */
   { EIB7_PDF_EnDat_AI1,     4, 4, 0, 0, NULL },         /* 7 */
   { EIB7_PDF_EnDat_AI2,     4, 4, 0, 0, NULL },         /* 8 */
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 9  (unused)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 10 (unused)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 11 (unused)*/
   /* auxiliary axis */
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 12 (dummy for trigger)*/
   { EIB7_PDF_AUXStatus,     2, 2, 0, 0, NULL },         /* 13 (will be referenced via EIB7_PDF_StatusWord) */
   { EIB7_PDF_AUXPosition,   8, 4, 0, 0, NULL },         /* 14 (will be referenced via EIB7_PDF_PositionData)*/
   { EIB7_PDF_AUXTimestamp,  4, 4, 0, 0, NULL },         /* 15 (will be referenced via EIB7_PDF_Timestamp)*/
   { EIB7_PDF_AUXRefPos,     8, 4, 0, 0, NULL },         /* 16 (will be referenced via EIB7_PDF_ReferencePos)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 17 (unused)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 18 (unused)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL },         /* 19 (unused)*/
   { EIB7_PDF_EndOfList,     0, 0, 0, 0, NULL }          /* 20 (unused)*/
};


/* this mask defines the valid entries for EIB_DataFieldDefinitionGlobal 
   the number of entries defines the number of "ones" in the mask.
   Do not include definitions for AuxAxis since this will be referenced
   via definitions for regular axis. */
#define DATA_FIELD_DESCR_GLOBAL_MASK 0x01FF




EIB7_ERR EIB_InitDataFields(EIB_Data *ed)
{
  ed->pkt_head.firstregion = NULL;
  ed->pkt_head.size = 0;

  return EIB7_NoError;
}


void EIB_CalculateOffsets(EIB_Data *ed)
{
   EIB_DataRegionDescription* region;
   EIB_DataFieldDescription* field;
   int offset = 0;
   int offset_raw = 0;

   region = ed->pkt_head.firstregion;

   while(region != NULL)
   {
      field = region->firstfield;
      while(field != NULL)
      {
         field->offset = offset;
         field->offset_raw = offset_raw;
         offset += field->size;
         offset_raw += field->size_raw;

         field = field->next;
      }

      region = region->next;
   }

   ed->posdatasize = offset;
   ed->posdatasize_raw = offset_raw;

}


void EIB_DeleteDataField(EIB_DataPacketDescription* packet)
{
   EIB_DataFieldDescription* field;
   EIB_DataRegionDescription* region;
   unsigned long i, n;

   for(i=0; i<packet->size; i++)
   {
      region = packet->firstregion;
      packet->firstregion = packet->firstregion->next;
      if(region != NULL)
      {
         for(n=0; n<region->size; n++)
         {
            field = region->firstfield;
            region->firstfield = region->firstfield->next;
            if(field != NULL)
               free(field);
         }
      }
      free(region);
   }

   packet->firstregion = NULL;
   packet->size = 0;
}



static void ConvertField(unsigned char *raw, int size_raw, void *dest, int dest_size)
{
   switch(size_raw)
   {
   case 12:
      {     /* always convert to 16 byte */
         ENCODER_POSITION *pos;

         pos = dest;

         *pos = 
            (ENCODER_POSITION)raw[0] +
            ((ENCODER_POSITION)raw[1] << 8 ) +
            ((ENCODER_POSITION)raw[2] << 16) +
            ((ENCODER_POSITION)raw[3] << 24) +
            ((ENCODER_POSITION)raw[4] << 32) +
            ((ENCODER_POSITION)raw[5] << 40) +
            ((ENCODER_POSITION)(raw[5] & 0x80 ? 0xffff : 0x0000) << 48);

         pos++;      /* increment pointer to second reference position */

         *pos = 
            (ENCODER_POSITION)raw[6] +
            ((ENCODER_POSITION)raw[7] << 8 ) +
            ((ENCODER_POSITION)raw[8] << 16) +
            ((ENCODER_POSITION)raw[9] << 24) +
            ((ENCODER_POSITION)raw[10] << 32) +
            ((ENCODER_POSITION)raw[11] << 40) +
            ((ENCODER_POSITION)(raw[11] & 0x80 ? 0xffff : 0x0000) << 48);
      }
      break;
   case 6:
      {     /* always convert to 8 byte */
         ENCODER_POSITION *pos;

         pos = dest;

         *pos = 
            (ENCODER_POSITION)raw[0] +
            ((ENCODER_POSITION)raw[1] << 8 ) +
            ((ENCODER_POSITION)raw[2] << 16) +
            ((ENCODER_POSITION)raw[3] << 24) +
            ((ENCODER_POSITION)raw[4] << 32) +
            ((ENCODER_POSITION)raw[5] << 40) +
            ((ENCODER_POSITION)(raw[5] & 0x80 ? 0xffff : 0x0000) << 48);
      }
      break;
   case 4:
      {
         if(dest_size == 8)
         {
            ENCODER_POSITION *pos;

            pos = dest;

            *pos = 
               (ENCODER_POSITION)raw[0] +
               ((ENCODER_POSITION)raw[1] << 8 ) +
               ((ENCODER_POSITION)raw[2] << 16) +
               ((ENCODER_POSITION)raw[3] << 24) +
               ((ENCODER_POSITION)(raw[3] & 0x80 ? 0xffffffff : 0x00000000) << 32);
         }
         else
         {
            unsigned long *val;
            val = dest;

            *val = 
               (unsigned long)raw[0] +
               ((unsigned long)raw[1] << 8 ) +
               ((unsigned long)raw[2] << 16) +
               ((unsigned long)raw[3] << 24);
         }
      }
      break;
   case 2:
      {
         unsigned short *val;
         val = dest;

         *val = 
            (unsigned short)raw[0] +
            ((unsigned short)raw[1] << 8 );
      }
      break;
   }
}
  
void EIB_ConvertPositionData(EIB_Data *ed, void *raw_data, void *data)
{
   EIB_DataRegionDescription* region;
   EIB_DataFieldDescription* field;

   region = ed->pkt_head.firstregion;

   while(region != NULL)
   {
      field = region->firstfield;
      while(field != NULL)
      {
         ConvertField((unsigned char *)raw_data + field->offset_raw,
               field->size_raw, (unsigned char *)data + field->offset, field->size);

         field = field->next;
      }

      region = region->next;
   }
}



int EIB_GetPacketConfigLength(EIB_DataPacketDescription* packet)
{
   int length = 0;
   EIB_DataRegionDescription* region;
   EIB_DataFieldDescription* field;

   region = packet->firstregion;

   while(region != NULL)
   {
      if(region->type != EIB7_DR_Global)
         length++;   /* for the axis number */

      field = region->firstfield;
      while(field != NULL)
      {
         length++;
         field = field->next;
      }

      region = region->next;
   }

   return length;
}


int EIB_ConvertPacketConfiguration(EIB_DataPacketDescription* packet, unsigned char* data, int len)
{
   int idx;
   int length;
   EIB_DataRegionDescription* region;
   EIB_DataFieldDescription* field;

   length = EIB_GetPacketConfigLength(packet);
   if(length < 0)
      return -1;

   if(len < length)
      return -1;     /* destination memory too small */

   idx = 0;
   region = packet->firstregion;

   while(region != NULL)
   {
      if(region->type != EIB7_DR_Global)
         data[idx++] = region->type & 0xFF;     /* add axis number */
                                                /* the global section does not need a number */

      field = region->firstfield;
      while(field != NULL)
      {
         switch(field->type)
         {
         case EIB7_PDF_TriggerCounter:
            data[idx++] = 0x10;
            break;
         case EIB7_PDF_StatusWord:
            data[idx++] = 0x11;
            break;
         case EIB7_PDF_PositionData:
            data[idx++] = 0x12;
            break;
         case EIB7_PDF_Timestamp:
            data[idx++] = 0x13;
            break;
         case EIB7_PDF_Analog:
            data[idx++] = 0x14;
            break;
         case EIB7_PDF_ReferencePos:
            data[idx++] = 0x15;
            break;
         case EIB7_PDF_DistCodedRef:
            data[idx++] = 0x17;
            break;
         case EIB7_PDF_EnDat_AI1:
            data[idx++] = 0x18;
            break;
         case EIB7_PDF_EnDat_AI2:
            data[idx++] = 0x19;
            break;
         case EIB7_PDF_AUXStatus:
            data[idx++] = 0x11;        // same as EIB7_PDF_StatusWord
            break;
         case EIB7_PDF_AUXPosition:
            data[idx++] = 0x12;        // same as EIB7_PDF_PositionData
            break;
         case EIB7_PDF_AUXTimestamp:
            data[idx++] = 0x13;        // same as EIB7_PDF_Timestamp
            break;
         case EIB7_PDF_AUXRefPos:
            data[idx++] = 0x15;        // same as EIB7_PDF_ReferencePos
            break;

         default:
            return -2;
            break;
         }

         field = field->next;
      }

      region = region->next;
   }

   return 0;
}


EIB7_ERR EIB_MakeDataField(EIB7_DataPacketSection* packet, unsigned long size, EIB_Data *ed)
{
   unsigned long i, n;
   EIB_DataRegionDescription* region;
   EIB_DataRegionDescription* it_region;
   EIB_DataFieldDescription* field;
   EIB_DataFieldDescription* it_field;

   for(i=0; i<size; i++)
   {
      region = (EIB_DataRegionDescription*)calloc(sizeof(EIB_DataRegionDescription), 1);
      if(region == NULL)
         return EIB7_OutOfMemory;

      region->type = packet[i].region;
      region->next = NULL;
      region->firstfield = NULL;
      region->size = 0;

      /* for each bit add one entry */
      for(n=0; n<(sizeof(packet[i].items)*8); n++)
      {
         if((packet[i].items & ((unsigned long)0x01 << n)) & (unsigned long)DATA_FIELD_DESCR_GLOBAL_MASK)
         {
            field = (EIB_DataFieldDescription*)calloc(sizeof(EIB_DataFieldDescription), 1);
            if(field == NULL)
               return EIB7_OutOfMemory;

            field->next = NULL;
            if(region->type == EIB7_DR_AUX)
            {
               /* auxiliary axis */
               field->type = EIB_DataFieldDefinitionGlobal[n+12].type;
               field->size = EIB_DataFieldDefinitionGlobal[n+12].size;
               field->size_raw = EIB_DataFieldDefinitionGlobal[n+12].size_raw;
            }
            else
            {
               /* regular axis */
               field->type = EIB_DataFieldDefinitionGlobal[n].type;
               field->size = EIB_DataFieldDefinitionGlobal[n].size;
               field->size_raw = EIB_DataFieldDefinitionGlobal[n].size_raw;
            }
            field->offset = 0;
            field->offset_raw = 0;

            /* insert new field description into list */
            it_field = region->firstfield;
            if(it_field == NULL)
               region->firstfield = field;
            else
            {
               while(it_field->next != NULL)
                  it_field = it_field->next;

               it_field->next = field;
            }
            region->size++;
         }
      }

      /* insert new region description into list */
      it_region = ed->pkt_head.firstregion;
      if(it_region == NULL)
         ed->pkt_head.firstregion = region;
      else
      {
         while(it_region->next != NULL)
            it_region = it_region->next;

         it_region->next = region;
      }
      ed->pkt_head.size++;
   }

   return EIB7_NoError;
}



