#include "rag.h"
#include <gtk/gtk.h>
#include <netinet/in.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
         GLOBAL VARIABLES
=================================================================================================*/
GtkLabel *gpLblOutput;
GtkWidget *gpBtnCpToCb, *gpMnuCpToCb;
GtkWidget *gpBtnClr, *gpMnuClr;

/*=================================================================================================
         LOCAL FUNCTION PROTOTYPES
=================================================================================================*/
STATUS ragGuiGetAllWidgets(GtkBuilder *pBuilder);
STATUS ragGuiShowInOutputLabel(U8 *pStr);

/*=================================================================================================
         LOCAL FUNCTION DEFINITIONS
=================================================================================================*/
STATUS ragGuiGetAllWidgets(GtkBuilder *pBuilder)
{
    STATUS rc = ERROR;

    do
    {
        gpLblOutput = GTK_LABEL(GTK_WIDGET(gtk_builder_get_object(pBuilder, "lblOutput")));
        if (gpLblOutput == NULL)
        {
            printf("RAG_GUI: failed to get 'lblOutput' label widget.\n");
            break;
        }

        gpBtnCpToCb = GTK_WIDGET(gtk_builder_get_object(pBuilder, "btnCopyToClipboard"));
        if (gpBtnCpToCb == NULL)
        {
            printf("RAG_GUI: failed to get 'btnCopyToClipboard' widget.\n");
            break;
        }

        gpMnuCpToCb = GTK_WIDGET(gtk_builder_get_object(pBuilder, "mnuCopyToClipboard"));
        if (gpMnuCpToCb == NULL)
        {
            printf("RAG_GUI: failed to get 'mnuCopyToClipboard' widget.\n");
            break;
        }

        gpBtnClr = GTK_WIDGET(gtk_builder_get_object(pBuilder, "btnClear"));
        if (gpBtnClr == NULL)
        {
            printf("RAG_GUI: failed to get 'btnClear' widget.\n");
            break;
        }

        gpMnuClr = GTK_WIDGET(gtk_builder_get_object(pBuilder, "mnuClear"));
        if (gpMnuClr == NULL)
        {
            printf("RAG_GUI: failed to get 'mnuClear' widget.\n");
            break;
        }

        rc = OK;
    }
    while(0);

    return rc;
}

STATUS ragGuiShowInOutputLabel(U8 *pStr)
{
    STATUS rc = ERROR;

    do
    {
        gtk_label_set_text(gpLblOutput, pStr);

        /* Since there is something in the output label, we need to enable the copy to clipboard
         * button and the corresponding menu item. */
        gtk_widget_set_sensitive(gpBtnCpToCb, TRUE);
        gtk_widget_set_sensitive(gpMnuCpToCb, TRUE);

        /* Since there is something in the output label, we need to enable the clear button and
         * the corresponding menu item. */
        gtk_widget_set_sensitive(gpBtnClr, TRUE);
        gtk_widget_set_sensitive(gpMnuClr, TRUE);

        rc = OK;
    }
    while(0);

    return rc;
}

/*=================================================================================================
         GTK SIGNAL HANDLER FUNCTION DEFINITIONS
=================================================================================================*/
G_MODULE_EXPORT void ragGuiClearOutputLabel(GtkButton *button, gpointer data)
{
    do
    {
        gtk_label_set_text(gpLblOutput, "");

        /* Since there is nothing anymore in the output label, we need to disable the copy to
         * clipboard button and menu item. */
        gtk_widget_set_sensitive(gpBtnCpToCb, FALSE);
        gtk_widget_set_sensitive(gpMnuCpToCb, FALSE);

        /* Since there is nothing anymore in the output label, we need to disable the clear button
         * and menu item. */
        gtk_widget_set_sensitive(gpBtnClr, FALSE);
        gtk_widget_set_sensitive(gpMnuClr, FALSE);
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomMacAddr(GtkButton *button, gpointer data)
{
    U8 macAddrBuf[MAC_ADDRBUFLEN] = {0};
    U8 macAddrStr[MAC_ADDRSTRLEN] = {0};

    do
    {
        getRandomMacAddress(macAddrBuf, sizeof(macAddrBuf));
        macAddrToStr(macAddrBuf, macAddrStr);

        if (ragGuiShowInOutputLabel(macAddrStr) != OK)
        {
            printf("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomIpv4Addr(GtkButton *button, gpointer data)
{
    U8 ipv4AddrBuf[INET_ADDRBUFLEN] = {0};
    U8 ipv4AddrStr[INET_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv4Address(ipv4AddrBuf, sizeof(ipv4AddrBuf));
        ipv4AddrToStr(ipv4AddrBuf, ipv4AddrStr);

        if (ragGuiShowInOutputLabel(ipv4AddrStr) != OK)
        {
            printf("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomIpv6Addr(GtkButton *button, gpointer data)
{
    U8 ipv6AddrBuf[INET6_ADDRBUFLEN] = {0};
    U8 ipv6AddrStr[INET6_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv6Address(ipv6AddrBuf, sizeof(ipv6AddrBuf));
        ipv6AddrToStr(ipv6AddrBuf, ipv6AddrStr);

        if (ragGuiShowInOutputLabel(ipv6AddrStr) != OK)
        {
            printf("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiCopyToClipboard(GtkButton *button, gpointer data)
{
    GtkClipboard *pClipboard;

    do
    {
        /* Get a reference to the clipboard object. */
        pClipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        if (pClipboard == NULL)
        {
            printf("RAG_GUI: 'gtk_clipboard_get' failed.\n");
            break;
        }

        /* Get the text from the output label and put it into the clipboard. The length can be
         * passed as '-1' since the text is NULL terminated. */
        gtk_clipboard_set_text(pClipboard, gtk_label_get_text(gpLblOutput), -1);
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiQuit(GtkButton *button, gpointer data)
{
    do
    {
        gtk_main_quit();
    }
    while(0);
}

/*=================================================================================================
         GLOBAL FUNCTION DEFINITIONS
=================================================================================================*/
STATUS ragLaunchGui(void)
{
    STATUS     rc = ERROR;
    GtkBuilder *pBuilder;
    GtkWidget  *pWindow;
    GError     *pError = NULL;
    int        dummy1 = 0;
    char       **dummy2 = NULL;

    do
    {
        /* Check if the glade file that defines the GUI exists. */
        if (access("ragGui.glade", R_OK) != 0)
        {
            printf("'ragGui.glade' not found, this file is needed to display the GUI.\n");
            break;
        }

        /* Init GTK+. */
        gtk_init(&dummy1, &dummy2);

        /* Create a new GtkBuilder object. */
        pBuilder = gtk_builder_new();

        /* Load UI from the glade file. */
        if (!(gtk_builder_add_from_file(pBuilder, "ragGui.glade", &pError)))
        {
            g_warning("%s", pError->message);
            g_free(pError);
            break;
        }

        /* Get the main window pointer. */
        pWindow = GTK_WIDGET(gtk_builder_get_object(pBuilder, "windowMain"));
        if (pWindow == NULL)
        {
            printf("RAG_GUI: failed to get 'windowMain' widget.\n");
            break;
        }

        /* Connect signals. */
        gtk_builder_connect_signals(pBuilder, NULL);

        /* Get references to all widgets that we'll need later. */
        if (ragGuiGetAllWidgets(pBuilder) != OK)
        {
            printf("RAG_GUI: 'ragGuiGetAllWidgets' failed.\n");
            break;
        }

        /* Destroy the builder object, since we don't need it anymore. */
        g_object_unref(G_OBJECT(pBuilder));

        /* Show the main window. All other widgets are automatically shown. */
        gtk_widget_show(pWindow);

        /* Start the main GTK loop. */
        gtk_main();

        rc = OK;
    }
    while(0);

    return rc;
}

#ifdef __cplusplus
}
#endif

