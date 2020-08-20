#include <stdio.h>
#include "hisdk_log.h"
#include "argtable3.h"

struct arg_lit *help, *version;
struct arg_end *end;

int test_func1()
{
	HISDK_LOG_INFO(LOG_DEBUG, "%s", "hello world!!");
	return 0;
}

int main(int argc, char *argv[])
{
    void *argtable[] = {
        help    = arg_lit0(NULL, "help", "display this help and exit"),
        version = arg_lit0(NULL, "version", "display version info and exit"),
        end     = arg_end(20),
    };

    int exitcode = 0;
    char progname[] = "example_hisdk_log";
    int nerrors;
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

    /* If the parser returned any errors then display them and exit */
    if (nerrors > 0)
    {
        /* Display the error details contained in the arg_end struct.*/
        arg_print_errors(stdout, end, progname);
        printf("Try '%s --help' for more information.\n", progname);
        exitcode = 1;
        goto exit;
    }

	hisdkLOGSetPrintDebugLogFlag(HISDK_LOG_DEBUG_ENABLE);//打印调试信息
	hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_TERM);  //保存打印信息到终端
    //hisdkLOGSetPrintLogPlaceFlag(HISDK_LOG_TO_FILE);//保存打印信息到文件
	hisdkLOGInit("log", 8000); //(logfilename, filemaxsize)

	HISDK_LOG_INFO(LOG_DEBUG, "%s", "Init log!!");

	test_func1();

	HISDK_LOG_INFO(LOG_DEBUG, "%s", "Destroy log!!");
	hisdkLOGDestroy();

exit:
    /* deallocate each non-null entry in argtable[] */
    arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
    return exitcode;
}