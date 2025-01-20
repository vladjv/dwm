/* Module to control keyboard lighting for Asus laptops within DWM using
 * asusctl's Aura control module.
 *
 * It is better to read the kernel data structures for keyboard and graphics but
 * since asusctl's tools need to be installed anyways to control ASUS ROG
 * features and there's not much overhead in spawning a process to parse
 * information, it's okay to just use them.
 */

#ifdef __ASUS_AURA__

#include <limits.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define ARRAY_SIZE(arr) (sizeof((arr)) / sizeof((arr)[0]))
#define ASUSCTL_PATH "/usr/local/bin/asusctl"

/* Asus Aura keyboard light modes */
const char *KBDL_STR[4] = {"Off", "Low", "Med", "High"};
static char *LEDBRIGHT_RE = "^.+brightness:\\s*(Off|Low|Med|High)";

typedef enum { OFF, LOW, MED, HIGH } KBLight;

/* Asus Aura keyboard control routines */
static int get_kblight();
static void set_kblight(const KBLight *);
static void decrease_kblight(const Arg *);
static void increase_kblight(const Arg *);

/* Graphic modes for Asus laptops with dedicated GPU */
static char *GMODE_STR[3] = {"Integrated", "Hybrid", "AsusMuxDgpu"};

typedef enum {
    INTEGRATED,
    HYBRID,
    DGPU,
    GMASK = INTEGRATED | HYBRID | DGPU
} GraphicMode;

/* Asus graphics mode control routines */
/*static int get_gmode(); // Pass by pointer would be better but nastier.*/
/*static void next_gmode(const Arg *);*/

int get_kblight() {
    int s;
    FILE *kb_p = NULL;
    regex_t led_re;
    regmatch_t pmatch[2];
    KBLight kb_light = -1;

    s = regcomp(&led_re, LEDBRIGHT_RE, REG_ICASE | REG_EXTENDED);

    if (s != 0)
        return -1;

    char kb_buf[_POSIX_PIPE_BUF];

    /* Retrieve current keyboard lighting mode */
    kb_p = popen(ASUSCTL_PATH " -k", "r");

    if (kb_p == NULL)
        return -1;

    if (fgets(kb_buf, 256, kb_p) == NULL) {
        regfree(&led_re);
        return -1;
    }

    s = regexec(&led_re, kb_buf, ARRAY_SIZE(pmatch), pmatch, 0);

    if (s == REG_NOMATCH) {
        if (kb_p != NULL)
            pclose(kb_p);

        if (led_re.re_nsub != 0)
            regfree(&led_re);
    }

    char led_mode[5];

    // Insert regmatch in `led_mode` buffer.
    snprintf(led_mode, 5, "%.*s", pmatch[1].rm_eo - pmatch[1].rm_so,
             kb_buf + pmatch[1].rm_so);

    for (KBLight s = OFF; kb_light < ARRAY_SIZE(KBDL_STR); s++) {
        if (strcmp(led_mode, KBDL_STR[s]) == 0) {
            kb_light = s;
            break;
        }
    }

    return kb_light;
}

void set_kblight(const KBLight *kblight) {
    /* Set changed keyboard light */
    const char *kbdcmd[] = {ASUSCTL_PATH, "-k", KBDL_STR[*kblight], NULL};

    if (fork() == 0) {
        setsid();
        execvp(kbdcmd[0], (char **)kbdcmd);
    }
}

void decrease_kblight() {
    KBLight curr_kblight = get_kblight();

    if (curr_kblight == -1)
        return;

    if (curr_kblight != OFF) {
        curr_kblight -= 1;
        set_kblight((const KBLight *)&curr_kblight);
    }
}

void increase_kblight() {
    KBLight curr_kblight = get_kblight();

    if (curr_kblight == -1)
        return;

    if (curr_kblight != HIGH) {
        curr_kblight += 1;
        set_kblight((const KBLight *)&curr_kblight);
    }
}

int get_gmode() {
    FILE *gm_p;
    GraphicMode gm;
    char gm_buf[_POSIX_PIPE_BUF];
    memset(gm_buf, 0, _POSIX_PIPE_BUF);

    /* Retrieve current graphics mode */
    gm_p = popen("/usr/bin/supergfxctl -g", "r");

    if (gm_p == NULL)
        return -1;

    if (fgets(gm_buf, _POSIX_PIPE_BUF, gm_p) == NULL)
        return -1;

    // Just check for first letter.
    // Probably better a `strcmp` but whatever. This
    // is faster and there's no collide in cases
    switch (*gm_buf) {
    case 'I':
        gm = INTEGRATED;
        break;
    case 'A':
        gm = DGPU;
        break;
    default:
        gm = HYBRID;
    }

    pclose(gm_p);
    return gm;
}

void next_gmode(const Arg *arg) {
    GraphicMode current_gmode = get_gmode();
    /* Obscure mode setting.
     * Just add one to GraphicMode's enum and wrap around supported values
     */
    if (current_gmode != -1) {
        GraphicMode next_gmode = (current_gmode + 1) % GMASK;

        char *kbdcmd[] = {"/usr/bin/supergfxctl", "-m", GMODE_STR[next_gmode],
                          NULL};

        if (fork() == 0) {
            setsid();
            execvp(kbdcmd[0], (char **)kbdcmd);
        }
    }
}
#endif
