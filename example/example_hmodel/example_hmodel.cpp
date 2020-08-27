#include <stdio.h>
#include "hisdk_log.h"
#include "argtable3.h"
#include "hmodel.h"

using namespace hisdk;

struct arg_lit *help;
struct arg_lit *version;
struct arg_file *outfile;
struct arg_file *infiles;
struct arg_end *end;

int test_func1()
{
    HISDK_LOG_INFO(LOG_DEBUG, "%s", "hello world!!");
    return 0;
}

int main(int argc, char *argv[])
{
    void *argtable[] = {
        help    = arg_lit0("h", "help",    "display this help and exit"),
        version = arg_lit0("v", "version", "display version info and exit"),
        outfile = arg_file0("o", NULL,"<output>",          "output file (default is \"-\")"),
        infiles = arg_filen(NULL,NULL,NULL,0,argc+2,       "input file(s)"),
        end     = arg_end(20)
    };
    int exitcode = 0;
    char progname[] = "example_hmodel";
    int nerrors;
    std::string modelfile_name;
    hisdkFile_t *file = NULL;
    hisdkStatType_t finfo;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n", progname);
        exitcode=1;
        goto fail;
    }

    /* set any command line default values prior to parsing */
    outfile->filename[0]="model.hmodel";

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);
    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto fail;
    }

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("List information about the FILE(s) "
               "(the current directory by default).\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exitcode = 0;
        goto fail;
    }
    
    /* special case: '--version' takes precedence error reporting */
    if (version->count > 0)
    {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        printf("September 2003, Stewart Heitmann\n");
        exitcode = 0;
        goto fail;
    }
        
    /* test hmodel serialization */
    if (outfile->count > 0)
    {
        HISDK_LOG_INFO(LOG_SYSTEM, "hmodel serialization, model file = %s...", outfile->filename[0]);
        modelfile_name = outfile->filename[0];
        

        std::vector<AddressListEntry> addresslist;
        addresslist.push_back(AddressListEntry(0,0,0,0));
        addresslist.push_back(AddressListEntry(1,0,0,0));
        addresslist.push_back(AddressListEntry(2,0,0,0));

        std::vector<MemoryListEntry> memorylist;
        memorylist.push_back(MemoryListEntry(0,0,0,0,0));
        memorylist.push_back(MemoryListEntry(1,0,0,0,0));
        memorylist.push_back(MemoryListEntry(2,0,0,0,0));

        std::vector<TaskListEntry> tasklist;
        tasklist.push_back(TaskListEntry(0));
        tasklist.push_back(TaskListEntry(1));
        tasklist.push_back(TaskListEntry(2));

        hModel model;
        model.setAddressListEntries(addresslist);
        model.setMemoryListEntries(memorylist);
        model.setTaskListEntries(tasklist);
        model.serialize();

        u64_t bufsize;
        model.getSerializedDataSize(&bufsize);
        u8_t *buf = new u8_t[bufsize];
        model.getSerializedData(buf);
        
        hisdkPortOsFopen(modelfile_name.c_str(), HISDK_OPEN_WRITE, &file);
        hisdkPortOsFwrite(file, buf, bufsize);
        hisdkPortOsFclose(file);

        HISDK_LOG_INFO(LOG_SYSTEM, "hmodel serialization, model size = %lu...", bufsize);
        if(buf != NULL)
            delete(buf);
    }
    else
    {
        /* test hmodel deserialization */
        if(infiles->count > 0)
        {
            HISDK_LOG_INFO(LOG_SYSTEM, "hmodel deserialization, model file = %s...", infiles->filename[0]);
            modelfile_name = infiles->filename[0];

            hisdkPortOsFopen(modelfile_name.c_str(), HISDK_OPEN_READ, &file);
            hisdkPortOsFstat(file, &finfo);
            size_t file_size = hisdkPortOsStatGetSize(&finfo);
            u8_t *buf = new u8_t[file_size];
            hisdkPortOsFseek(file, 0, HisdkSeek_Set);
            size_t actually_read = 0;
            hisdkPortOsFread(file, buf, file_size, &actually_read);
            hisdkPortOsFclose(file);

            hModel model;
            model.deserializeFrom(buf);

            const std::vector<AddressListEntry> & addresslist = model.getAddressListEntries();
            const std::vector<MemoryListEntry>  & memorylist  = model.getMemoryListEntries();
            const std::vector<TaskListEntry>    & tasklist    = model.getTaskListEntries();

            HISDK_LOG_INFO(LOG_SYSTEM, "hmodel deserialization, al.size=%lu, ml.size=%lu, tl.size=%lu", 
                addresslist.size(), memorylist.size(), tasklist.size());
        }
        else
        {
            HISDK_LOG_INFO(LOG_SYSTEM, "parameter error, program exit...");
        }
    }

fail:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}