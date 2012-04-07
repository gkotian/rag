#include "ra.h"
#include <gtk/gtk.h>
#include <netinet/in.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
         GLOBAL VARIABLES
=================================================================================================*/
GtkWidget *gpTextView;
GtkWidget *gpLabel;
GtkWidget *gpBtnCpToCb, *gpMnuCpToCb;
GtkWidget *gpBtnClr, *gpMnuClr;

/*=================================================================================================
         LOCAL FUNCTION PROTOTYPES
=================================================================================================*/
STATUS raGuiGetAllWidgets(GtkBuilder *pBuilder);
STATUS raGuiShowInTextView(U8 *pStr);

/*=================================================================================================
         LOCAL FUNCTION DEFINITIONS
=================================================================================================*/
STATUS raGuiGetAllWidgets(GtkBuilder *pBuilder)
{
    STATUS rc = ERROR;

    do
    {
        gpTextView = GTK_WIDGET(gtk_builder_get_object(pBuilder, "textviewOutput"));
        if (gpTextView == NULL)
        {
            printf("RA_GUI: failed to get 'textviewOutput' widget.\n");
            break;
        }

        gpLabel = GTK_WIDGET(gtk_builder_get_object(pBuilder, "label1"));
        if (gpLabel == NULL)
        {
            printf("RA_GUI: failed to get 'label1' widget.\n");
            break;
        }

        gpBtnCpToCb = GTK_WIDGET(gtk_builder_get_object(pBuilder, "btnCopyToClipboard"));
        if (gpBtnCpToCb == NULL)
        {
            printf("RA_GUI: failed to get 'btnCopyToClipboard' widget.\n");
            break;
        }

        gpMnuCpToCb = GTK_WIDGET(gtk_builder_get_object(pBuilder, "mnuCopyToClipboard"));
        if (gpMnuCpToCb == NULL)
        {
            printf("RA_GUI: failed to get 'mnuCopyToClipboard' widget.\n");
            break;
        }

        gpBtnClr = GTK_WIDGET(gtk_builder_get_object(pBuilder, "btnClear"));
        if (gpBtnClr == NULL)
        {
            printf("RA_GUI: failed to get 'btnClear' widget.\n");
            break;
        }

        gpMnuClr = GTK_WIDGET(gtk_builder_get_object(pBuilder, "mnuClear"));
        if (gpMnuClr == NULL)
        {
            printf("RA_GUI: failed to get 'mnuClear' widget.\n");
            break;
        }

        rc = OK;
    }
    while(0);

    return rc;
}

STATUS raGuiShowInTextView(U8 *pStr)
{
    STATUS        rc = ERROR;
    GtkTextBuffer *pTextBuffer;
    GtkLabel *pLabel;

    do
    {
        pLabel = GTK_LABEL(gpLabel);
        gtk_label_set_text(pLabel, pStr);
        pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gpTextView));
        if (pTextBuffer == NULL)
        {
            printf("RA_GUI: 'gtk_text_view_get_buffer' failed.\n");
            break;
        }

        gtk_text_buffer_set_text(pTextBuffer, pStr, -1);

/* TODO: need to implement the 'raGuiCopyToClipboard' function. Till then, keep the copy to
 * clipboard button and menu item disabled. */
#if 0
        /* Since there is something in the textview, we need to enable the copy to clipboard
         * button and menu item. */
        gtk_widget_set_sensitive(gpBtnCpToCb, TRUE);
        gtk_widget_set_sensitive(gpMnuCpToCb, TRUE);
#endif

        /* Since there is something in the textview, we need to enable the clear button and menu
         * item. */
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
G_MODULE_EXPORT void raGuiClearTextView(GtkButton *button, gpointer data)
{
    GtkTextBuffer *pTextBuffer;

    do
    {
        pTextBuffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(gpTextView));
        if (pTextBuffer == NULL)
        {
            printf("RA_GUI: 'gtk_text_view_get_buffer' failed.\n");
            break;
        }

        gtk_text_buffer_set_text(pTextBuffer, "", -1);

        /* Since there is nothing anymore in the textview, we need to disable the copy to clipboard
         * button and menu item. */
        gtk_widget_set_sensitive(gpBtnCpToCb, FALSE);
        gtk_widget_set_sensitive(gpMnuCpToCb, FALSE);

        /* Since there is nothing anymore in the textview, we need to disable the copy to clipboard
         * button and menu item. */
        gtk_widget_set_sensitive(gpBtnClr, FALSE);
        gtk_widget_set_sensitive(gpMnuClr, FALSE);
    }
    while(0);
}

G_MODULE_EXPORT void raGuiGetRandomMacAddr(GtkButton *button, gpointer data)
{
    U8 macAddrBuf[MAC_ADDRBUFLEN] = {0};
    U8 macAddrStr[MAC_ADDRSTRLEN] = {0};

    do
    {
        getRandomMacAddress(macAddrBuf, sizeof(macAddrBuf));
        macAddrToStr(macAddrBuf, macAddrStr);

        if (raGuiShowInTextView(macAddrStr) != OK)
        {
            printf("RA_GUI: 'raGuiShowInTextView' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void raGuiGetRandomIpv4Addr(GtkButton *button, gpointer data)
{
    U8 ipv4AddrBuf[INET_ADDRBUFLEN] = {0};
    U8 ipv4AddrStr[INET_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv4Address(ipv4AddrBuf, sizeof(ipv4AddrBuf));
        ipv4AddrToStr(ipv4AddrBuf, ipv4AddrStr);

        if (raGuiShowInTextView(ipv4AddrStr) != OK)
        {
            printf("RA_GUI: 'raGuiShowInTextView' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void raGuiGetRandomIpv6Addr(GtkButton *button, gpointer data)
{
    U8 ipv6AddrBuf[INET6_ADDRBUFLEN] = {0};
    U8 ipv6AddrStr[INET6_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv6Address(ipv6AddrBuf, sizeof(ipv6AddrBuf));
        ipv6AddrToStr(ipv6AddrBuf, ipv6AddrStr);

        if (raGuiShowInTextView(ipv6AddrStr) != OK)
        {
            printf("RA_GUI: 'raGuiShowInTextView' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void raGuiCopyToClipboard(GtkButton *button, gpointer data)
{
    do
    {
        /* TODO */
    }
    while(0);
}

G_MODULE_EXPORT void raGuiQuit(GtkButton *button, gpointer data)
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
STATUS raLaunchGui(void)
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
        if (access("raGui.glade", R_OK) != 0)
        {
            printf("'raGui.glade' not found, this file is needed to display the GUI.\n");
            break;
        }
     
        /* Init GTK+. */
        gtk_init(&dummy1, &dummy2);
     
        /* Create a new GtkBuilder object. */
        pBuilder = gtk_builder_new();

        /* Load UI from the glade file. */
        if (!(gtk_builder_add_from_file(pBuilder, "raGui.glade", &pError)))
        {
            g_warning("%s", pError->message);
            g_free(pError);
            break;
        }
     
        /* Get the main window pointer. */
        pWindow = GTK_WIDGET(gtk_builder_get_object(pBuilder, "windowRaMain"));
        if (pWindow == NULL)
        {
            printf("RA_GUI: failed to get 'windowRaMain' widget.\n");
            break;
        }
     
        /* Connect signals. */
        gtk_builder_connect_signals(pBuilder, NULL);

        /* Get references to all widgets that we'll need later. */
        if (raGuiGetAllWidgets(pBuilder) != OK)
        {
            printf("RA_GUI: 'raGuiGetAllWidgets' failed.\n");
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

