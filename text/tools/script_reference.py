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
        names.append(name)
        print(stem, len(keep), "characters", n, "cases")
    with open(os.path.join(OUT, "index.txt"), "w") as fh:
        fh.write("\n".join(names) + "\n")
    write_licences(copyrights)


if __name__ == "__main__":
    main()
