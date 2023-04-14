#include <ctype.h>

gchar* lTrim(gchar s[]) {
	gint i = 0;
	while (s[i] == ' ') i++;
	if (i > 0) strcpy(&s[0], &s[i]);
	return s;
}

gchar* rTrim(gchar s[]) {
	gint i = strlen(s) - 1;
	while (s[i] == ' ') i--;
	s[i + 1] = '\0';
	return s;
}

gchar* trim(gchar s[]) {
	rTrim(lTrim(s));
	gint i, x;
	for (i = x = 0; s[i]; ++i)
		if (!isspace(s[i]) || (i > 0 && !isspace(s[i - 1])))
			s[x++] = s[i];
	s[x] = '\0';
	return s;
}

//------

gboolean zero_to_nine_keys_callback(GtkWidget* widget, GdkEventKey* event)
{
    (void)widget;
    guint keys[12] =
    {
        GDK_KEY_0, GDK_KEY_1, GDK_KEY_2, GDK_KEY_3, GDK_KEY_4,
        GDK_KEY_5, GDK_KEY_6, GDK_KEY_7, GDK_KEY_8, GDK_KEY_9,
        GDK_KEY_BackSpace, GDK_KEY_Tab
    };

    for (guint i = 0; i < 12; i++)
    {
        if (event->keyval == keys[i])
        {
            return FALSE;
        }
    }

    return TRUE;
}