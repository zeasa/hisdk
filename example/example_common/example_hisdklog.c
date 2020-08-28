#include <stdio.h>
#include "hisdk_log.h"
#include "argtable3.h"

struct arg_lit *help;
struct arg_lit *version;
struct arg_lit *debug;
struct arg_file *logfile;
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
        debug   = arg_lit0("d", "debug",   "enable log of debug information"),
        logfile = arg_file0("f", "logfile", "<output>", "log file (default is \"-\")"),
        end     = arg_end(20)
    };

    int exitcode = 0;
    char progname[] = "example_hisdklog";
    int nerrors;

    /* verify the argtable[] entries were allocated sucessfully */
    if (arg_nullcheck(argtable) != 0)
    {
        /* NULL entries were detected, some allocations must have failed */
        printf("%s: insufficient memory\n", progname);
        exitcode=1;
        goto exit;
    }

    /* set any command line default values prior to parsing */
    logfile->filename[0]="log";

    /* Parse the command line as defined by argtable[] */
    nerrors = arg_parse(argc,argv,argtable);

    /* special case: '--help' takes precedence over error reporting */
    if (help->count > 0)
    {
        printf("Usage: %s", progname);
        arg_print_syntax(stdout, argtable, "\n");
        printf("List information about the FILE(s) "
               "(the current directory by default).\n\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
        exitcode = 0;
        goto exit;
    }
    
    /* special case: '--version' takes precedence error reporting */
    if (version->count > 0)
    {
        printf("'%s' example program for the \"argtable\" command line argument parser.\n",progname);
        printf("September 2003, Stewart Heitmann\n");
        exitcode = 0;
        goto exit;
    }
        
    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto exit;
    }
    
    /* special case: '--version' takes precedence error reporting */
    if (debug->count > 0)
    {
        hisdkLOGSetPrintDebugLogFlag(HISDK_LOG_DEBUG_ENABLE);//打印调试信息
    }
    else
    {
        hisdkLOGSetPrintDebugLogFlag(HISDK_LOG_DEBUG_DISABLE);//打印调试信息
    }

    if(logfile->count > 0)
    {
        hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_FILE);//保存打印信息到文件
    }
    else
    {
        hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_TERM);  //保存打印信息到终端
    }
    
    hisdkLOGInit(logfile->filename[0], 8000); //(logfilename, filemaxsize)

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "Init log!!");

    test_func1();

    HISDK_LOG_INFO(LOG_DEBUG, "%s", "Destroy log!!");
    hisdkLOGDestroy();

exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}