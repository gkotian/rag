/*
Copyright (c) 2012 Gautam Kotian

This file is part of "Random Address Generator".

"Random Address Generator" is free software: you can redistribute it and/or modify it under the
terms of the GNU General Public License as published by the Free Software Foundation, either
version 3 of the License, or (at your option) any later version.

"Random Address Generator" is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with "Random Address
Generator". If not, see <http://www.gnu.org/licenses/>.
*/

#ifdef __cplusplus
extern "C" {
#endif

/*=================================================================================================
         HEADER FILES
=================================================================================================*/
#include "rag.h"
#include <gtk/gtk.h>
#include <netinet/in.h>
#include <unistd.h>

#define RAG_LICENSE_TEXT "\"Random Address Generator\" is free software: you can redistribute it \
and/or modify it under the terms of the GNU General Public License as published by the Free \
Software Foundation, either version 3 of the License, or (at your option) any later version.\n\n\
\"Random Address Generator\" is distributed in the hope that it will be useful, but WITHOUT ANY \
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR \
PURPOSE.\nSee the GNU General Public License for more details.\n\nYou should have received a copy \
of the GNU General Public License along with \"Random Address Generator\". If not, see \
<http://www.gnu.org/licenses/>."

/*=================================================================================================
         STRUCTURES
=================================================================================================*/
/* Structure containing widgets used in the program. */
typedef struct
{
    GtkWidget *pWindow;
    GtkLabel  *pLblOutput;
    GtkWidget *pBtnCopyToClipboard;
    GtkWidget *pMnuCopyToClipboard;
    GtkWidget *pBtnClear;
    GtkWidget *pMnuClear;

} RAG_WIDGETS_T;

/*=================================================================================================
         LOCAL FUNCTION PROTOTYPES
=================================================================================================*/
STATUS ragGuiGetAllWidgets(GtkBuilder *pBuilder, RAG_WIDGETS_T *pWidgets);
STATUS ragGuiShowInOutputLabel(U8 *pStr, RAG_WIDGETS_T *pWidgets);

/*=================================================================================================
         LOCAL FUNCTION DEFINITIONS
=================================================================================================*/
STATUS ragGuiGetAllWidgets(GtkBuilder *pBuilder, RAG_WIDGETS_T *pWidgets)
{
    STATUS rc = ERROR;

    do
    {
        pWidgets->pWindow = GTK_WIDGET(gtk_builder_get_object(pBuilder, "windowMain"));
        if (pWidgets->pWindow == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'windowMain' widget.\n");
            break;
        }

        pWidgets->pLblOutput = GTK_LABEL(GTK_WIDGET(gtk_builder_get_object(pBuilder, "lblOutput")));
        if (pWidgets->pLblOutput == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'lblOutput' label widget.\n");
            break;
        }

        pWidgets->pBtnCopyToClipboard = GTK_WIDGET(gtk_builder_get_object(pBuilder,
                                                                         "btnCopyToClipboard"));
        if (pWidgets->pBtnCopyToClipboard == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'btnCopyToClipboard' widget.\n");
            break;
        }

        pWidgets->pMnuCopyToClipboard = GTK_WIDGET(gtk_builder_get_object(pBuilder,
                                                                         "mnuCopyToClipboard"));
        if (pWidgets->pMnuCopyToClipboard == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'mnuCopyToClipboard' widget.\n");
            break;
        }

        pWidgets->pBtnClear = GTK_WIDGET(gtk_builder_get_object(pBuilder, "btnClear"));
        if (pWidgets->pBtnClear == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'btnClear' widget.\n");
            break;
        }

        pWidgets->pMnuClear = GTK_WIDGET(gtk_builder_get_object(pBuilder, "mnuClear"));
        if (pWidgets->pMnuClear == NULL)
        {
            RAG_LOG("RAG_GUI: failed to get 'mnuClear' widget.\n");
            break;
        }

        rc = OK;
    }
    while(0);

    return rc;
}

STATUS ragGuiShowInOutputLabel(U8 *pStr, RAG_WIDGETS_T *pWidgets)
{
    STATUS rc = ERROR;

    do
    {
        gtk_label_set_text(pWidgets->pLblOutput, pStr);

        /* Since there is something in the output label, we need to enable the copy to clipboard
         * button and the corresponding menu item. */
        gtk_widget_set_sensitive(pWidgets->pBtnCopyToClipboard, TRUE);
        gtk_widget_set_sensitive(pWidgets->pMnuCopyToClipboard, TRUE);

        /* Since there is something in the output label, we need to enable the clear button and
         * the corresponding menu item. */
        gtk_widget_set_sensitive(pWidgets->pBtnClear, TRUE);
        gtk_widget_set_sensitive(pWidgets->pMnuClear, TRUE);

        rc = OK;
    }
    while(0);

    return rc;
}

/*=================================================================================================
         GTK SIGNAL HANDLER FUNCTION DEFINITIONS
=================================================================================================*/
G_MODULE_EXPORT void ragGuiClearOutputLabel(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    do
    {
        gtk_label_set_text(pWidgets->pLblOutput, "");

        /* Since there is nothing anymore in the output label, we need to disable the copy to
         * clipboard button and menu item. */
        gtk_widget_set_sensitive(pWidgets->pBtnCopyToClipboard, FALSE);
        gtk_widget_set_sensitive(pWidgets->pMnuCopyToClipboard, FALSE);

        /* Since there is nothing anymore in the output label, we need to disable the clear button
         * and menu item. */
        gtk_widget_set_sensitive(pWidgets->pBtnClear, FALSE);
        gtk_widget_set_sensitive(pWidgets->pMnuClear, FALSE);
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomMacAddr(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    U8 macAddrBuf[MAC_ADDRBUFLEN] = {0};
    U8 macAddrStr[MAC_ADDRSTRLEN] = {0};

    do
    {
        getRandomMacAddress(macAddrBuf, sizeof(macAddrBuf));
        macAddrToStr(macAddrBuf, macAddrStr);

        if (ragGuiShowInOutputLabel(macAddrStr, pWidgets) != OK)
        {
            RAG_LOG("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomIpv4Addr(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    U8 ipv4AddrBuf[INET_ADDRBUFLEN] = {0};
    U8 ipv4AddrStr[INET_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv4Address(ipv4AddrBuf, sizeof(ipv4AddrBuf));
        ipv4AddrToStr(ipv4AddrBuf, ipv4AddrStr);

        if (ragGuiShowInOutputLabel(ipv4AddrStr, pWidgets) != OK)
        {
            RAG_LOG("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiGetRandomIpv6Addr(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    U8 ipv6AddrBuf[INET6_ADDRBUFLEN] = {0};
    U8 ipv6AddrStr[INET6_ADDRSTRLEN] = {0};

    do
    {
        getRandomIpv6Address(ipv6AddrBuf, sizeof(ipv6AddrBuf));
        ipv6AddrToStr(ipv6AddrBuf, ipv6AddrStr);

        if (ragGuiShowInOutputLabel(ipv6AddrStr, pWidgets) != OK)
        {
            RAG_LOG("RAG_GUI: 'ragGuiShowInOutputLabel' failed.\n");
            break;
        }
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiCopyToClipboard(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    GtkClipboard *pClipboard;

    do
    {
        /* Get a reference to the clipboard object. */
        pClipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);
        if (pClipboard == NULL)
        {
            RAG_LOG("RAG_GUI: 'gtk_clipboard_get' failed.\n");
            break;
        }

        /* Get the text from the output label and put it into the clipboard. The length can be
         * passed as '-1' since the text is NULL terminated. */
        gtk_clipboard_set_text(pClipboard, gtk_label_get_text(pWidgets->pLblOutput), -1);
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiQuit(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    do
    {
        gtk_main_quit();
    }
    while(0);
}

G_MODULE_EXPORT void ragGuiAbout(GtkButton *button, RAG_WIDGETS_T *pWidgets)
{
    static const gchar * const authors[] =
    {
        "Gautam Kotian <gautam.kotian@gmail.com>",
        NULL
    };
    static const gchar comments[] = "Generates random MAC, IPv4 & IPv6 addresses";
    static const gchar copyright[] = "Copyright \xc2\xa9 2012 Gautam Kotian";
    static const gchar license[] = RAG_LICENSE_TEXT;
    static const gchar logo_icon_name[] = GTK_STOCK_PROPERTIES;
    static const gchar program_name[] = "Random Address Generator";
    static const gchar version[] = "(v0.1)";
    static const gchar website[] = "";

    do
    {
        gtk_show_about_dialog(GTK_WINDOW(pWidgets->pWindow),
                              "authors", authors,
                              "comments", comments,
                              "copyright", copyright,
                              "license", license,
                              "logo-icon-name", logo_icon_name,
                              "program-name", program_name,
                              "version", version,
                              "website", website,
                              "wrap-license", TRUE,
                              NULL); 
    }
    while(0);
}

/*=================================================================================================
         GLOBAL FUNCTION DEFINITIONS
=================================================================================================*/
STATUS ragLaunchGui(int *pArgc, char **pArgv)
{
    STATUS        rc = ERROR;
    GtkBuilder    *pBuilder;
    GError        *pError = NULL;
    RAG_WIDGETS_T *pWidgets;

    do
    {
        /* Check if the builder XML file that defines the GUI exists. */
        if (access(BUILDER_XML_FILE, R_OK) != 0)
        {
            RAG_LOG("'%s' not found, this file is needed to display the GUI.\n", BUILDER_XML_FILE);
            break;
        }

        /* Allocate memory needed by the widgets structure. */
        pWidgets = g_slice_new(RAG_WIDGETS_T);

        /* Init GTK+ libraries. */
        gtk_init(pArgc, &pArgv);

        /* Create a new GtkBuilder object. */
        pBuilder = gtk_builder_new();
        if (pBuilder == NULL)
        {
            RAG_LOG("RAG_GUI: 'gtk_builder_new' failed.\n");
            break;
        }

        /* Load UI from the builder XML file. */
        if (!(gtk_builder_add_from_file(pBuilder, BUILDER_XML_FILE, &pError)))
        {
            g_warning("%s", pError->message);
            g_free(pError);
            break;
        }

        /* Get references to all widgets that we'll need later. */
        if (ragGuiGetAllWidgets(pBuilder, pWidgets) != OK)
        {
            RAG_LOG("RAG_GUI: 'ragGuiGetAllWidgets' failed.\n");
            break;
        }

        /* Connect signals, passing the widgets structure as user data. */
        gtk_builder_connect_signals(pBuilder, pWidgets);

        /* Destroy the builder object, since we don't need it anymore. */
        g_object_unref(G_OBJECT(pBuilder));

        /* Show the main window. All other widgets are automatically shown. */
        gtk_widget_show(pWidgets->pWindow);

        /* Start the main GTK loop. */
        gtk_main();

        /* Free memory allocated for the widgets strucure. */
        g_slice_free(RAG_WIDGETS_T, pWidgets);

        rc = OK;
    }
    while(0);

    return rc;
}

#ifdef __cplusplus
}
#endif

