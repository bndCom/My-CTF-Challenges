#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <stdint.h>
#include <openssl/sha.h>

#define SECTOR_SIZE 512

#pragma pack(push, 1)
// Structure for an MBR partition entry
typedef struct {
    unsigned char status;             
    unsigned char chs_start[3];       
    unsigned char partition_type;     
    unsigned char chs_end[3];         
    unsigned int lba_start;           
    unsigned int num_sectors;         
} MBRPartitionEntry;

// Structure for the Master Boot Record (MBR)
typedef struct {
    unsigned char boot_code[446];     // 446 bytes
    MBRPartitionEntry partitions[4]; // 4 × 16 = 64 bytes
    unsigned short signature;         // 0xAA55
} MBR;

// Structure for the FAT32 boot sector
typedef struct {
    unsigned char jump[3];
    unsigned char oem_name[8];
    unsigned short bytes_per_sector;
    unsigned char sectors_per_cluster;
    unsigned short reserved_sectors;
    unsigned char num_fats;
    unsigned short num_root_entries;
    unsigned short total_sectors_16;
    unsigned char media;
    unsigned short fat_size_16;
    unsigned short sectors_per_track;
    unsigned short num_heads;
    unsigned int hidden_sectors;
    unsigned int total_sectors_32;
    unsigned int fat_size_32;
    unsigned short ext_flags;
    unsigned short fs_version;
    unsigned int root_cluster;
    unsigned short fs_info;
    unsigned short backup_boot_sector;
    unsigned char reserved[12];
    unsigned char drive_number;
    unsigned char reserved1;
    unsigned char boot_signature;
    unsigned int volume_id;
    unsigned char volume_label[11];
    unsigned char fs_type[8];
    unsigned char boot_code[420];
    unsigned short boot_sector_signature;
} FAT32BootSector;

// Structure for a FAT32 directory entry
typedef struct {
    unsigned char name[11];
    unsigned char attributes;
    unsigned char nt_reserved;
    unsigned char creation_time_tenths;
    unsigned short creation_time;
    unsigned short creation_date;
    unsigned short access_date;
    unsigned short high_cluster;
    unsigned short modification_time;
    unsigned short modification_date;
    unsigned short low_cluster;
    unsigned int file_size;
} FAT32DirectoryEntry;
#pragma pack(pop)

unsigned char* ReadSector(const char* physical_disk, int sector_num) {
    unsigned char* buffer = malloc(SECTOR_SIZE);
    if (buffer == NULL) {
        perror("Memory allocation error");
        return NULL;
    }

    FILE *disk = fopen(physical_disk, "rb");
    if (disk == NULL) {
        perror("Error opening disk");
        free(buffer);
        return NULL;
    }

    if (fseek(disk, (long)SECTOR_SIZE * sector_num, SEEK_SET) != 0) {
        perror("fseek error");
        free(buffer);
        fclose(disk);
        return NULL;
    }

    if (fread(buffer, SECTOR_SIZE, 1, disk) != 1) {
        perror("fread error");
        free(buffer);
        fclose(disk);
        return NULL;
    }

    fclose(disk);
    return buffer;
}

int main() {
    char* physical_disk = "final.img";
    FILE* disk = fopen(physical_disk, "rb+");
    if (disk == NULL) {
        perror("Error opening disk");
        return -1;
    }

    // Printing MBR info
    unsigned char* buffer = ReadSector(physical_disk, 0);
    if (buffer == NULL) {
        fprintf(stderr, "Error reading sector %d\n", 0);
        return -1;
    }

    MBR* mbr = (MBR*)buffer;

    if (mbr->signature != 0xAA55) {
        fprintf(stderr, "Invalid MBR signature\n");
        free(buffer);
        return -1;
    }

    // To locate the exact part where the disk got corrupted, we need to confirm some hash values. What is the MBR Boot Code (Routine) SHA1 hash? -> 421cbcadd2db047228046350d80d6433a39741d1
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA1(mbr->boot_code, sizeof(mbr->boot_code), hash);

    printf("SHA1 of the MBR boot sector: ");
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        printf("%02x", hash[i]);
    }
    printf("\n");

    // What is the partition's volume ID -> 0x2530e884
    static int partition_count = 1;

    printf("********************** MBR Partition Table of %s **********************\n\n", physical_disk);
    printf("| %-9s | %-6s | %-6s | %-4s | %-13s | %-16s | %-11s |\n",
           "Partition", "Offset", "Status", "Type", "LBA Start", "Number of Sectors", "Size (MB)");
    printf("|-----------|--------|--------|------|--------------|------------------|-------------|\n");

    MBRPartitionEntry* p = &mbr->partitions[0];

    float size_mb = (float)p->num_sectors * SECTOR_SIZE / (1024.0 * 1024);

    char partition_name[10];
    snprintf(partition_name, sizeof(partition_name), "loop0p%d", partition_count++);

    printf("| %-9s | %-6d |  0x%02x  | 0x%02x | %-12u | %-16u | %-11f |\n",
           partition_name,
           0,
           p->status,
           p->partition_type,
           p->lba_start,
           p->num_sectors,
           size_mb);

    // Modify the LBA start of the first partition
    unsigned int new_lba_start = 2048; // Example new LBA start value
    p->lba_start = new_lba_start;

    // Save the modified MBR back to the disk
    fseek(disk, 0, SEEK_SET);
    if (fwrite(buffer, SECTOR_SIZE, 1, disk) != 1) {
        perror("Error writing modified MBR to disk");
        free(buffer);
        fclose(disk);   
        return -1;
    }

    printf("LBA start of the first partition modified to: %u\n", new_lba_start);

    // Printing first partition (FAT32) info
    FAT32BootSector boot_sector;
    fseek(disk, (long)p->lba_start * SECTOR_SIZE, SEEK_SET);
    fread(&boot_sector, sizeof(FAT32BootSector), 1, disk);

    if (memcmp(boot_sector.fs_type, "FAT32   ", 8) != 0) {
        printf("This partition does not appear to be a FAT32 partition\n");
        fclose(disk);
        return 0;
    }

    if (boot_sector.sectors_per_cluster == 0 ||
        boot_sector.num_fats == 0 ||
        boot_sector.bytes_per_sector == 0) {
        printf("Invalid values in the FAT32 boot sector\n");
        fclose(disk);
        return 0;
    }

    uint8_t sectors_per_cluster  = boot_sector.sectors_per_cluster;
    uint16_t reserved_sectors    = boot_sector.reserved_sectors;
    uint8_t num_fats             = boot_sector.num_fats;
    uint32_t sectors_per_fat     = boot_sector.fat_size_32;
    uint32_t root_cluster        = boot_sector.root_cluster;

    printf("\n--- FAT32 Boot Sector Information ---\n");
    printf("Sector size             : %d bytes\n", SECTOR_SIZE);
    printf("Sectors per cluster     : %u\n", sectors_per_cluster);
    printf("Reserved sectors        : %u\n", reserved_sectors);
    printf("Number of FATs          : %u\n", num_fats);
    printf("FAT size                : %u sectors\n", sectors_per_fat);
    printf("Root cluster            : %u\n", root_cluster);

    uint32_t lba_first_cluster = p->lba_start + reserved_sectors + num_fats * sectors_per_fat;
    printf("[+] LBA address of the first cluster: %u\n", lba_first_cluster);

    printf("Volume ID: %08x\n", boot_sector.volume_id);

    // Getting LBA and first sector of a file
    // What is the LBA of the first cluster of the file file_0008.bin -> 3978

    fseek(disk, lba_first_cluster * boot_sector.bytes_per_sector, SEEK_SET);

    printf("********************** Root Directory Content **********************\n\n");
    printf("Name         | Size (sectors) | Size (KB) | First Cluster  | LBA First Cluster  |\n");
    printf("-------------|----------------|-----------|----------------|--------------------|\n");

    FAT32DirectoryEntry entry;
    while (fread(&entry, sizeof(FAT32DirectoryEntry), 1, disk) == 1) {
        if (entry.name[0] == 0x00) {
             break;
        }
        if (entry.name[0] == 0xE5 || entry.attributes == 0x0F) {
            continue;
        }

        unsigned int first_cluster = (entry.high_cluster << 16) | entry.low_cluster;
        if (first_cluster == 0) {
            first_cluster = 2;
        }

        unsigned int lba_first_cluster_entry = lba_first_cluster + ((first_cluster - 2) * boot_sector.sectors_per_cluster);

        printf("%-11.11s | %17u | %11.3f | %16u | %20u |\n",
           entry.name,
           entry.file_size == 0 ? 0 : (entry.file_size + boot_sector.bytes_per_sector - 1) / boot_sector.bytes_per_sector,
           (float)entry.file_size / 1024,
           first_cluster,
           lba_first_cluster_entry);
    }

    fclose(disk);

    free(buffer);
    return 0;
}
