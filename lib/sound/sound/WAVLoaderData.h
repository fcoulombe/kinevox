/*
void Load_Wave_File(char *fname)
{
    FILE *fp;

    fp = fopen(fname,"rb);
    if (fp)
    {
        BYTE id[4], *sound_buffer; //four bytes to hold 'RIFF'
        DWORD size; //32 bit value to hold file size
        short format_tag, channels, block_align, bits_per_sample; //our 16 values
        DWORD format_length, sample_rate, avg_bytes_sec, data_size, i; //our 32 bit values

        fread(id, sizeof(BYTE), 4, fp); //read in first four bytes
        if (!strcmp(id, "RIFF"))
        { //we had 'RIFF' let's continue
            fread(size, sizeof(DWORD), 1, fp); //read in 32bit size value
            fread(id, sizeof(BYTE), 4, fp); //read in 4 byte string now
            if (!strcmp(id,"WAVE"))
            { //this is probably a wave file since it contained "WAVE"
                fread(id, sizeof(BYTE), 4, fp); //read in 4 bytes "fmt ";
                fread(&format_length, sizeof(DWORD),1,fp);
                fread(&format_tag, sizeof(short), 1, fp); //check mmreg.h (i think?) for other
                                                              // possible format tags like ADPCM
                fread(&channels, sizeof(short),1,fp); //1 mono, 2 stereo
                fread(&sample_rate, sizeof(DWORD), 1, fp); //like 44100, 22050, etc...
                fread(&avg_bytes_sec, sizeof(short), 1, fp); //probably won't need this
                fread(&block_align, sizeof(short), 1, fp); //probably won't need this
                fread(&bits_per_sample, sizeof(short), 1, fp); //8 bit or 16 bit file?
                fread(id, sizeof(BYTE), 4, fp); //read in 'data'
                fread(&data_size, sizeof(DWORD), 1, fp); //how many bytes of sound data we have
                sound_buffer = (BYTE *) malloc (sizeof(BYTE) * data_size); //set aside sound buffer space
                fread(sound_buffer, sizeof(BYTE), data_size, fp); //read in our whole sound data chunk
            }
            else
                printf("Error: RIFF file but not a wave file\n");
        }
        else
            printf("Error: not a RIFF file\n");
    }
}
*/
