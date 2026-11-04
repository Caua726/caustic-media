#!/usr/bin/env python3
# text/tools/script_reference.py — fonts for the writing systems with shapers of
# their own, cut down to their scripts, and texts in them shaped by HarfBuzz, for
# shape_test.cst. text/testdata/scripts/<font>.<ext> and <font>.shape.ref, in the
# format shape_reference.py describes; index.txt names them.
#
# Development tool: needs uharfbuzz and fontTools, and the Noto fonts of a Linux
# distribution, which are under the SIL Open Font License. The texts are words
# (wordfreq's lists where it has the language, and some written here), and
# strings made up from the script's own characters, with a fixed seed: well-formed
# or not, HarfBuzz's answer is what to hold the shaper to. The fonts are renamed
# "Caustic Test ...", their licence in text/testdata/licenses/scripts.txt.
#
#   uv run --python 3.13 --with uharfbuzz --with fonttools --with wordfreq text/tools/script_reference.py
import os
import random
import re

import uharfbuzz as hb
from fontTools import subset
from fontTools.ttLib import TTFont

try:
    from wordfreq import top_n_list
except ImportError:
    top_n_list = None

HERE = os.path.dirname(os.path.abspath(__file__))
LICENCES = os.path.join(HERE, "..", "testdata", "licenses", "scripts.txt")
OFL_TEXT_FROM = "/usr/share/licenses/inter-font/LICENSE.txt"
OUT = os.path.join(HERE, "..", "testdata", "scripts")
NOTO = "/usr/share/fonts/noto"
NOTO_CJK = "/usr/share/fonts/noto-cjk"
KOREAN_FACE = 1    # of the collection: Noto Sans CJK KR
SCRIPTS_TXT = os.path.expanduser("~/.cache/caustic-media/registry/Scripts.txt")

BOT = 1
ZWJ, ZWNJ = "‍", "‌"

# Written out: what the lists do not have.
WORDS = {
    "Arab": "السلام عليكم كتاب محمد الله لا إله إلا الله مدرسة بيت قلم شَمْس كِتَابٌ لِلّٰه الْعَرَبِيَّة فارسی اردو پاکستان ی ک ګ",
    "Syrc": "ܫܠܡܐ ܟܬܒܐ ܐܠܗܐ ܡܠܟܐ ܥܠܡܐ ܐܒܐ ܒܝܬܐ ܕܪܫܐ ܐܡܐ",
    "Deva": "नमस्ते हिन्दी भारत क्ष त्र ज्ञ श्री कर्म धर्म कृष्ण रिश्ता र्क र्कि ड़ क़ ट्र ट्ट द्ध द्व प्र्य स्त्री ॐ",
    "Beng": "বাংলা নমস্কার ক্ষ জ্ঞ র্ক কর্ম শ্রী কৌ কো র‍্য বিদ্যালয় আমার সোনার",
    "Guru": "ਪੰਜਾਬੀ ਸਤਿ ਸ੍ਰੀ ਅਕਾਲ ਗੁਰਮੁਖੀ ਕ੍ਰਿਪਾ ਪ੍ਰੇਮ ਧੰਨਵਾਦ",
    "Gujr": "ગુજરાતી નમસ્તે ક્ષ જ્ઞ શ્રી કર્મ પ્રેમ વિદ્યા",
    "Orya": "ଓଡ଼ିଆ ନମସ୍କାର କ୍ଷ ଜ୍ଞ ଶ୍ରୀ କର୍ମ ପ୍ରେମ କୋ କୌ",
    "Taml": "தமிழ் வணக்கம் க்ஷ ஸ்ரீ கொ கோ கௌ நன்றி பள்ளி",
    "Telu": "తెలుగు నమస్కారం క్ష జ్ఞ శ్రీ కర్మ ప్రేమ కై కొ విద్య",
    "Knda": "ಕನ್ನಡ ನಮಸ್ಕಾರ ಕ್ಷ ಜ್ಞ ಶ್ರೀ ಕರ್ಮ ಪ್ರೇಮ ಕೊ ಕೋ ವಿದ್ಯೆ",
    "Mlym": "മലയാളം നമസ്കാരം ക്ഷ ജ്ഞ ശ്രീ കര്‍മ്മം പ്രേമം കൊ കോ വിദ്യ",
    "Thai": "สวัสดี ภาษาไทย น้ำ ทำ ค่ำ น้ำใจ ปี่ ปู่ ญู ฐู ป่ำ เกาะ",
    "Laoo": "ສະບາຍດີ ພາສາລາວ ນ້ຳ ຄຳ ທຳ ບໍ່",
    "Hang": "안녕하세요 한국어 각 가 각 각 하〮 〮 ᄀ가ᅡ",
    "Khmr": "សួស្តី ភាសាខ្មែរ ខ្ញុំ ក្រុម ស្រី កៅ កើ ប៉ុន ព៌ ស្ត្រី",
    "Mymr": "မင်္ဂလာပါ မြန်မာ ကျွန်တော် သင်္ဘော ကြ ကျ ကွ ကှ ေက ဿ နှစ် ကြိုး",
    "Tibt": "བོད་ཡིག བཀྲ་ཤིས་བདེ་ལེགས སྐད་ཡིག སྒྲུབ བསྒྲུབས",
    "Sinh": "සිංහල ආයුබෝවන් ශ්‍රී ක්‍ෂ ක්‍ර කො කෝ",
    "Bali": "ᬩᬲᬩᬮᬶ ᬓ᭄ᬱ ᬳᬦᬘᬭᬓ ᬓᭂ ᬓᭀ",
    "Java": "ꦧꦱꦗꦮ ꦲꦤꦕꦫꦏ ꦏ꧀ꦱ ꦏꦺꦴ",
    "Nkoo": "ߒߞߏ ߞߊ߲߬ ߓߊߡߊ߲߬ߞߊ߲",
    "Mong": "ᠮᠣᠩᠭᠣᠯ ᠪᠢᠴᠢᠭ ᠠ᠋ ᠢ᠌ ᠰᠠᠢᠨ",
    "Adlm": "𞤆𞤵𞤤𞤢𞤪 𞤀𞤣𞤤𞤢𞤥",
}

# Font and language per script: the shaper they go to is the point.
FONTS = [
    ("Arab", "rtl", "ar", "NotoSansArabic-Regular.ttf"), ("Arab", "rtl", "ur", "NotoNastaliqUrdu-Regular.ttf"),
    ("Syrc", "rtl", None, "NotoSansSyriac-Regular.ttf"),
    ("Deva", "ltr", "hi", "NotoSansDevanagari-Regular.ttf"), ("Beng", "ltr", "bn", "NotoSansBengali-Regular.ttf"),
    ("Guru", "ltr", "pa", "NotoSansGurmukhi-Regular.ttf"), ("Gujr", "ltr", "gu", "NotoSansGujarati-Regular.ttf"),
    ("Orya", "ltr", "or", "NotoSansOriya-Regular.ttf"), ("Taml", "ltr", "ta", "NotoSansTamil-Regular.ttf"),
    ("Telu", "ltr", "te", "NotoSansTelugu-Regular.ttf"), ("Knda", "ltr", "kn", "NotoSansKannada-Regular.ttf"),
    ("Mlym", "ltr", "ml", "NotoSansMalayalam-Regular.ttf"),
    ("Thai", "ltr", "th", "NotoSansThai-Regular.ttf"), ("Laoo", "ltr", "lo", "NotoSansLao-Regular.ttf"),
    ("Hang", "ltr", "ko", "NotoSansCJK-Regular.ttc"),
    ("Khmr", "ltr", "km", "NotoSansKhmer-Regular.ttf"), ("Mymr", "ltr", "my", "NotoSansMyanmar-Regular.ttf"),
    ("Tibt", "ltr", "bo", "NotoSerifTibetan-Regular.ttf"), ("Sinh", "ltr", "si", "NotoSansSinhala-Regular.ttf"),
    ("Bali", "ltr", None, "NotoSansBalinese-Regular.ttf"), ("Java", "ltr", None, "NotoSansJavanese-Regular.ttf"),
    ("Nkoo", "rtl", None, "NotoSansNKo-Regular.ttf"), ("Mong", "ltr", None, "NotoSansMongolian-Regular.ttf"),
    ("Adlm", "rtl", None, "NotoSansAdlam-Regular.ttf"), ("Mand", "rtl", None, "NotoSansMandaic-Regular.ttf"),
    ("Mani", "rtl", None, "NotoSansManichaean-Regular.ttf"), ("Phag", "ltr", None, "NotoSansPhagsPa-Regular.ttf"),
    ("Phlp", "rtl", None, "NotoSansPsalterPahlavi-Regular.ttf"), ("Rohg", "rtl", None, "NotoSansHanifiRohingya-Regular.ttf"),
    ("Sogd", "rtl", None, "NotoSansSogdian-Regular.ttf"),
    ("Batk", "ltr", None, "NotoSansBatak-Regular.ttf"), ("Brah", "ltr", None, "NotoSansBrahmi-Regular.ttf"),
    ("Bugi", "ltr", None, "NotoSansBuginese-Regular.ttf"), ("Buhd", "ltr", None, "NotoSansBuhid-Regular.ttf"),
    ("Cakm", "ltr", None, "NotoSansChakma-Regular.ttf"), ("Cham", "ltr", None, "NotoSansCham-Regular.ttf"),
    ("Dupl", "ltr", None, "NotoSansDuployan-Regular.ttf"), ("Gran", "ltr", None, "NotoSansGrantha-Regular.ttf"),
    ("Hano", "ltr", None, "NotoSansHanunoo-Regular.ttf"), ("Kali", "ltr", None, "NotoSansKayahLi-Regular.ttf"),
    ("Khar", "rtl", None, "NotoSansKharoshthi-Regular.ttf"), ("Kthi", "ltr", None, "NotoSansKaithi-Regular.ttf"),
    ("Lana", "ltr", None, "NotoSansTaiTham-Regular.ttf"), ("Lepc", "ltr", None, "NotoSansLepcha-Regular.ttf"),
    ("Limb", "ltr", None, "NotoSansLimbu-Regular.ttf"), ("Mtei", "ltr", None, "NotoSansMeeteiMayek-Regular.ttf"),
    ("Modi", "ltr", None, "NotoSansModi-Regular.ttf"), ("Mult", "ltr", None, "NotoSansMultani-Regular.ttf"),
    ("Newa", "ltr", None, "NotoSansNewa-Regular.ttf"), ("Rjng", "ltr", None, "NotoSansRejang-Regular.ttf"),
    ("Saur", "ltr", None, "NotoSansSaurashtra-Regular.ttf"), ("Shrd", "ltr", None, "NotoSansSharada-Regular.ttf"),
    ("Sidd", "ltr", None, "NotoSansSiddham-Regular.ttf"), ("Sund", "ltr", None, "NotoSansSundanese-Regular.ttf"),
    ("Sylo", "ltr", None, "NotoSansSylotiNagri-Regular.ttf"), ("Tagb", "ltr", None, "NotoSansTagbanwa-Regular.ttf"),
    ("Tale", "ltr", None, "NotoSansTaiLe-Regular.ttf"), ("Takr", "ltr", None, "NotoSansTakri-Regular.ttf"),
    ("Tavt", "ltr", None, "NotoSansTaiViet-Regular.ttf"), ("Tfng", "ltr", None, "NotoSansTifinagh-Regular.ttf"),
    ("Tglg", "ltr", None, "NotoSansTagalog-Regular.ttf"), ("Tirh", "ltr", None, "NotoSansTirhuta-Regular.ttf"),
    ("Egyp", "ltr", None, "NotoSansEgyptianHieroglyphs-Regular.ttf"),
]

ALIASES = {}


def script_chars():
    """Each ISO 15924 code's characters, from Scripts.txt and its aliases."""
    aliases = {}
    path = os.path.join(os.path.dirname(SCRIPTS_TXT), "PropertyValueAliases.txt")
    for line in open(path, encoding="utf-8"):
        m = re.match(r"sc\s*;\s*(\w+)\s*;\s*(\w+)", line)
        if m:
            aliases[m.group(2)] = m.group(1)
    out = {}
    for line in open(SCRIPTS_TXT, encoding="utf-8"):
        line = line.split("#")[0].strip()
        if not line:
            continue
        cps, name = [p.strip() for p in line.split(";")]
        a, _, b = cps.partition("..")
        code = aliases.get(name)
        if code:
            out.setdefault(code, []).extend(range(int(a, 16), int(b or a, 16) + 1))
    return out


def cut_down(path, chars, out):
    font = source(path)
    have = font.getBestCmap()
    keep = sorted(u for u in chars if u in have)
    # Of Hangul the jamo, and of its syllables a slice: what is left out of the
    # font is what a shaper has to take apart, and the rest are like these.
    if path.endswith(".ttc"):
        keep = [u for u in keep if u < 0xAC00 or u >= 0xD7B0 or u < 0xAC00 + 28 * 21 * 2]
    for extra in (0x20, 0xA0, 0x200C, 0x200D, 0x25CC, 0x2010, 0x2011, 0x640, 0xFE00):
        if extra in have:
            keep.append(extra)
    options = subset.Options()
    options.layout_features = ["*"]
    options.layout_scripts = ["*"]
    options.name_IDs = [0, 1, 2, 3, 4, 6, 13, 14]
    options.notdef_outline = True
    options.glyph_names = False
    options.hinting = False
    sub = subset.Subsetter(options)
    sub.populate(unicodes=keep)
    sub.subset(font)
    # A subset is a Modified Version in the licence's terms: renamed, as
    # make_testdata.py does its own.
    for rec in font["name"].names:
        if rec.nameID in (1, 4):
            rec.string = rec.toUnicode().replace("Noto", "Caustic Test")
        if rec.nameID in (3, 6):
            rec.string = rec.toUnicode().replace("Noto", "CausticTest")
    font.save(out)
    return keep


def source(path):
    # The source's own date kept: made again, a font is the same bytes.
    if path.endswith(".ttc"):
        return TTFont(path, fontNumber=KOREAN_FACE, recalcTimestamp=False)
    return TTFont(path, recalcTimestamp=False)


def write_licences(copyrights):
    # One licence over them all: each font's own copyright line, then its text.
    text = open(OFL_TEXT_FROM, encoding="utf-8").read()
    body = text[text.index("This Font Software is licensed"):]
    with open(LICENCES, "w", encoding="utf-8") as f:
        for line in sorted(set(copyrights)):
            f.write(line + "\n")
        f.write("\nSubset and renamed \"Caustic Test ...\" for caustic-media's tests;\n")
        f.write("see text/tools/script_reference.py.\n\n")
        f.write(body)


def made_up(rng, keep, n):
    letters = [u for u in keep if u > 0x7F]
    out = []
    for _ in range(n):
        out.append("".join(chr(rng.choice(letters)) for _ in range(rng.randint(2, 7))))
    return out


def words_of(code, lang):
    words = WORDS.get(code, "").split()
    if top_n_list is not None and lang:
        try:
            words += [w for w in top_n_list(lang, 300) if w.isalpha() or code != "Arab"]
        except LookupError:
            pass
    return words


def shape(blob, text, code, direction, lang, flags):
    face = hb.Face(blob, 0)
    font = hb.Font(face)
    font.scale = (face.upem, face.upem)
    buf = hb.Buffer()
    buf.add_utf8(text.encode("utf-8"), 0, -1)
    buf.direction = direction
    buf.script = code
    if lang:
        buf.language = lang
    buf.flags = hb.BufferFlags(flags)
    hb.shape(font, buf, {})
    return ["%d %d %d %d %d %d" % (i.codepoint, i.cluster, p.x_advance, p.y_advance, p.x_offset, p.y_offset)
            for i, p in zip(buf.glyph_infos or [], buf.glyph_positions or [])]


def write_reference(name, stem, code, direction, lang, texts):
    blob = hb.Blob.from_file_path(os.path.join(OUT, name))
    lines = ["# %s shaped by HarfBuzz %s; see text/tools/script_reference.py" % (name, hb.version_string())]
    n = 0
    for t in texts:
        data = t.encode("utf-8")
        glyphs = shape(blob, t, code, direction, lang, BOT)
        lines += ["case " + code + " " + str(n), "face 0", "text " + " ".join("%02x" % b for b in data),
                  "item 0 -1", "props %s %s %s" % (direction, code, lang or "-"), "flags start", "features 0",
                  "variations 0", "glyphs %d" % len(glyphs)] + glyphs
        n += 1
    with open(os.path.join(OUT, stem + ".shape.ref"), "w") as fh:
        fh.write("\n".join(lines) + "\n")
    return n


# --- Fonts made here: what no Noto font is ---

def made_font(path, chars):
    # A box for each character, none with a table of layout: told apart by width,
    # for the positions as well as the glyphs to say which was chosen.
    from fontTools.fontBuilder import FontBuilder
    from fontTools.pens.ttGlyphPen import TTGlyphPen
    fb = FontBuilder(1000, isTTF=True)
    chars = sorted(set(chars))
    names = [".notdef"] + ["u%04X" % c for c in chars]
    fb.setupGlyphOrder(names)
    fb.setupCharacterMap({c: "u%04X" % c for c in chars})
    glyphs = {}
    metrics = {}
    for i, n in enumerate(names):
        width = 300 + (i * 7) % 400
        # Its side bearing and height its own too: a place worked out from them shows it.
        left = 10 + (i * 11) % 60
        top = 400 + (i * 13) % 300
        bottom = (i * 17) % 90 - 40
        pen = TTGlyphPen(None)
        pen.moveTo((left, bottom))
        pen.lineTo((left, top))
        pen.lineTo((width - 20, top))
        pen.lineTo((width - 20, bottom))
        pen.closePath()
        glyphs[n] = pen.glyph()
        metrics[n] = (width, left)
    fb.setupGlyf(glyphs)
    fb.setupHorizontalMetrics(metrics)
    fb.setupHorizontalHeader(ascent=800, descent=-200)
    fb.setupNameTable({"familyName": "Caustic Test Made", "styleName": "Regular"})
    fb.setupOS2()
    fb.setupPost()
    fb.save(path)


THAI_CONSONANTS = [0xE01, 0xE1B, 0xE1D, 0xE1F, 0xE0D, 0xE10, 0xE0E, 0xE0F, 0xE2E, 0xE2F, 0xE40, 0x61]
THAI_MARKS = [0xE31, 0xE34, 0xE35, 0xE36, 0xE37, 0xE47, 0xE4D, 0xE4E, 0xE38, 0xE39, 0xE3A, 0xE48, 0xE49, 0xE4A, 0xE4B, 0xE4C]
# The private use forms of Windows' fonts, and of Apple's.
THAI_WINDOWS = list(range(0xF700, 0xF71B))
THAI_APPLE = list(range(0xF884, 0xF89F))


def thai_texts():
    # Every consonant under every mark, and two and three of them, the second
    # the kind of each, with SARA AM among them: every state and every move.
    out = []
    for c in THAI_CONSONANTS:
        for m in THAI_MARKS:
            out.append(chr(c) + chr(m))
            out.append(chr(c) + chr(m) + "\u0e33")
            for k in THAI_MARKS:
                out.append(chr(c) + chr(m) + chr(k))
    rng = random.Random("Thai made")
    for _ in range(300):
        out.append(chr(rng.choice(THAI_CONSONANTS)) + "".join(chr(rng.choice(THAI_MARKS)) for _ in range(rng.randint(3, 5))))
    return out


def forms_singles():
    # The presentation forms that are one letter.
    import unicodedata
    out = []
    for u in list(range(0xFB50, 0xFE00)) + list(range(0xFE70, 0xFEFF)):
        d = unicodedata.decomposition(chr(u)).split()
        if len(d) == 2 and d[0].startswith("<"):
            out.append("%x" % u)
    return out


ARABIC_LETTERS = list(range(0x621, 0x64B)) + [0x67E, 0x686, 0x698, 0x6AF, 0x6A9, 0x6CC, 0x6C1, 0x6D2]
ARABIC_MARKS = list(range(0x64B, 0x653)) + [0x670]
ZW = [0x200C, 0x200D]


def arabic_texts():
    # Letters joined every way, the lams before the alefs, marks and the zero
    # widths between them.
    rng = random.Random("Arabic made")
    out = []
    lam, alefs = 0x644, [0x622, 0x623, 0x625, 0x627]
    for a in alefs:
        out += [chr(lam) + chr(a), chr(0x628) + chr(lam) + chr(a), chr(lam) + chr(0x651) + chr(a), chr(lam) + chr(0x200D) + chr(a)]
    # Every pair and three that has a ligature, alone and between letters that join.
    import unicodedata
    seen = set()
    for u in list(range(0xFB50, 0xFE00)) + list(range(0xFE70, 0xFEFF)):
        d = unicodedata.decomposition(chr(u)).split()
        if len(d) >= 3 and d[0].startswith("<"):
            parts = [int(x, 16) for x in d[1:]]
            if all(unicodedata.category(chr(p)) == "Lo" for p in parts) and tuple(parts) not in seen:
                seen.add(tuple(parts))
                word = "".join(chr(p) for p in parts)
                out += [word, chr(0x628) + word, word + chr(0x628), chr(0x628) + word + chr(0x628)]
    for parts in ((0x644, 0x644, 0x647), (0x644, 0x645, 0x62C)):
        word = "".join(chr(p) for p in parts)
        out += [word, chr(0x628) + word, word + chr(0x628), chr(0x628) + word + chr(0x628)]
    for m in ARABIC_MARKS:
        out.append(chr(0x628) + chr(m) + chr(0x651))
        out.append(chr(0x628) + chr(0x651) + chr(m))
        out.append(chr(0x628) + chr(0x640) + chr(m) + chr(0x651))
    for _ in range(500):
        t = []
        for _ in range(rng.randint(1, 7)):
            t.append(rng.choice(ARABIC_LETTERS))
            if rng.random() < 0.2:
                t.append(rng.choice(ARABIC_MARKS))
            if rng.random() < 0.1:
                t.append(rng.choice(ZW))
        out.append("".join(chr(c) for c in t))
    return out


HANGUL_L = list(range(0x1100, 0x1113)) + [0x1113, 0x115E, 0xA960]
HANGUL_V = list(range(0x1161, 0x1176)) + [0x1176, 0x11A2, 0xD7B0]
HANGUL_T = list(range(0x11A8, 0x11C3)) + [0x11C3, 0x11FF, 0xD7CB]
HANGUL_SYLLABLES = [0xAC00, 0xAC01, 0xAC1C, 0xAC1D, 0xAE68, 0xD788, 0xD7A3, 0xB098]
HANGUL_OTHERS = [0x302E, 0x302F, 0x61, 0x25CC, 0x20]


def hangul_texts():
    # A syllable written every way, whole and in jamo, and jamo that make none,
    # tone marks after each.
    rng = random.Random("Hangul made")
    out = []
    for _ in range(600):
        t = []
        for _ in range(rng.randint(1, 3)):
            kind = rng.randint(0, 4)
            if kind == 0:
                t += [rng.choice(HANGUL_L), rng.choice(HANGUL_V)]
            elif kind == 1:
                t += [rng.choice(HANGUL_L), rng.choice(HANGUL_V), rng.choice(HANGUL_T)]
            elif kind == 2:
                t += [rng.choice(HANGUL_SYLLABLES)]
                if rng.random() < 0.5:
                    t.append(rng.choice(HANGUL_T))
            elif kind == 3:
                t += [rng.choice(HANGUL_L + HANGUL_V + HANGUL_T)]
            else:
                t += [rng.choice(HANGUL_OTHERS)]
        out.append("".join(chr(c) for c in t))
    return out


def marks_of_every_kind():
    # Two of each category of mark and combining class there is.
    import unicodedata
    seen = {}
    for u in range(0x300, 0x1E950):
        c = chr(u)
        cat = unicodedata.category(c)
        if cat in ("Mn", "Mc", "Me"):
            key = (cat, unicodedata.combining(c))
            if len(seen.setdefault(key, [])) < 2:
                seen[key].append(u)
    return sorted(u for v in seen.values() for u in v)


def mark_texts(marks, bases):
    rng = random.Random("Marks made")
    out = []
    for b in bases:
        for m in marks:
            out.append(chr(b) + chr(m))
    for _ in range(400):
        out.append(chr(rng.choice(bases)) + "".join(chr(rng.choice(marks)) for _ in range(rng.randint(2, 4))))
    for m in marks:
        out.append(chr(m))
        out.append(chr(m) + chr(rng.choice(marks)))
    return out


def made_fonts(names):
    def made(stem, chars, code, direction, lang, texts):
        name = stem + ".ttf"
        made_font(os.path.join(OUT, name), chars)
        n = write_reference(name, stem, code, direction, lang, texts)
        names.append(name)
        print(stem, n, "cases")
    thai = list(range(0xE01, 0xE3B)) + list(range(0xE3F, 0xE5C)) + [0x20, 0x61]
    made("thai-pua-windows", thai + THAI_WINDOWS, "Thai", "ltr", "th", thai_texts())
    made("thai-pua-apple", thai + THAI_APPLE, "Thai", "ltr", "th", thai_texts())
    made("thai-pua-none", thai, "Thai", "ltr", "th", thai_texts())
    # Marks with nothing to place them: every kind, by the shapers that place them.
    marks = marks_of_every_kind()
    latin = [0x61, 0x6D, 0x69]
    made("marks-latin", marks + latin + [0x20, 0x25CC], "Latn", "ltr", "en", mark_texts(marks, latin))
    hebrew = [0x5D0, 0x5D1, 0x5D9]
    made("marks-hebrew", marks + hebrew + [0x20, 0x25CC], "Hebr", "rtl", "he", mark_texts(marks, hebrew))
    hangul = [0xAC00, 0xAC01, 0x61]
    made("marks-hangul", marks + hangul + [0x20, 0x25CC], "Hang", "ltr", "ko", mark_texts(marks, hangul))
    # Arabic with no features: drawn through what presentation forms it has.
    arabic = list(range(0x621, 0x653)) + [0x670, 0x67E, 0x686, 0x698, 0x6A9, 0x6AF, 0x6C1, 0x6CC, 0x6D2, 0x640, 0x20, 0x25CC]
    forms_b = list(range(0xFB50, 0xFBB2)) + list(range(0xFBD3, 0xFD3E))
    forms_a = [c for c in range(0xFE70, 0xFEFD) if c not in (0xFE75,)] + [0xF201, 0xF211]
    lam_alef = list(range(0xFEF5, 0xFEFD))
    made("arabic-forms", arabic + forms_b + forms_a, "Arab", "rtl", "ar", arabic_texts())
    made("arabic-forms-no-lam-alef", arabic + forms_b + [c for c in forms_a if c not in lam_alef], "Arab", "rtl", "ar", arabic_texts())
    made("arabic-forms-none", arabic, "Arab", "rtl", "ar", arabic_texts())
    # Ligatures with none of the single forms they are made from.
    singles = {int(c, 16) for c in forms_singles()}
    made("arabic-forms-ligatures-only", arabic + [c for c in forms_b + forms_a if c not in singles], "Arab", "rtl", "ar", arabic_texts())
    # Hangul with no features: syllables and no jamo, jamo and no syllables,
    # both, and no dotted circle for a tone mark alone.
    jamo = HANGUL_L + HANGUL_V + HANGUL_T
    syllables = list(range(0xAC00, 0xAC00 + 28 * 21 * 2)) + [0xAE68, 0xB098]
    rest = [0x302E, 0x302F, 0x61, 0x20]
    made("hangul-syllables", syllables + rest + [0x25CC], "Hang", "ltr", "ko", hangul_texts())
    made("hangul-jamo", jamo + rest + [0x25CC], "Hang", "ltr", "ko", hangul_texts())
    made("hangul-both", syllables + jamo + rest + [0x25CC], "Hang", "ltr", "ko", hangul_texts())
    made("hangul-both-no-circle", syllables + jamo + rest, "Hang", "ltr", "ko", hangul_texts())


def main():
    os.makedirs(OUT, exist_ok=True)
    chars = script_chars()
    names = []
    copyrights = []
    for code, direction, lang, filename in FONTS:
        path = os.path.join(NOTO_CJK if filename.endswith(".ttc") else NOTO, filename)
        # Each script's own: none left out for want of its font.
        if not os.path.exists(path):
            raise SystemExit("no " + filename)
        stem = os.path.splitext(filename)[0]
        name = stem + (".otf" if filename.endswith(".ttc") else ".ttf")
        keep = cut_down(path, chars.get(code, []), os.path.join(OUT, name))
        copyrights.append(source(path)["name"].getDebugName(0) or "")
        rng = random.Random(code)
        texts = words_of(code, lang) + made_up(rng, keep, 300)
        # A sentence, and a text starting with what needs a base.
        if len(texts) > 4:
            texts.append(" ".join(texts[:4]))
        n = write_reference(name, stem, code, direction, lang, texts)
        names.append(name)
        print(stem, len(keep), "characters", n, "cases")
    made_fonts(names)
    with open(os.path.join(OUT, "index.txt"), "w") as fh:
        fh.write("\n".join(names) + "\n")
    write_licences(copyrights)


if __name__ == "__main__":
    main()
