#include "dictionary.h"
#include "settings.h"

#include <fstream>
#include <iostream>
#include <string>

namespace seneca {

    static std::string trim(const std::string& s) {
        size_t b = 0;
        while (b < s.size() && (s[b] == ' ' || s[b] == '\t' || s[b] == '\r')) ++b;

        size_t e = s.size();
        while (e > b && (s[e - 1] == ' ' || s[e - 1] == '\t' || s[e - 1] == '\r')) --e;

        return s.substr(b, e - b);
    }

    static std::string unquote(const std::string& s) {
        if (s.size() >= 2 && s.front() == '"' && s.back() == '"') {
            return s.substr(1, s.size() - 2);
        }
        return s;
    }

    static bool parseCSV3(const std::string& line, std::string& f1, std::string& f2, std::string& f3) {
        f1.clear(); f2.clear(); f3.clear();

        std::string fields[3];
        int fi = 0;
        bool inQuotes = false;

        for (size_t i = 0; i < line.size(); ++i) {
            const char c = line[i];

            if (c == '"') {
                inQuotes = !inQuotes;
                fields[fi] += c;
            }
            else if (c == ',' && !inQuotes) {
                ++fi;
                if (fi > 2) break;
            }
            else {
                fields[fi] += c;
            }
        }

        if (fi < 2) return false;

        f1 = trim(fields[0]);
        f2 = trim(fields[1]);
        f3 = trim(fields[2]);
        return true;
    }

    static PartOfSpeech mapPOS(const std::string& posRaw) {
        const std::string p = trim(posRaw);

        if (p == "n." || p == "n. pl.") return PartOfSpeech::Noun;
        if (p == "adv.") return PartOfSpeech::Adverb;
        if (p == "a.") return PartOfSpeech::Adjective;
        if (p == "v." || p == "v. i." || p == "v. t." || p == "v. t. & i.") return PartOfSpeech::Verb;
        if (p == "prep.") return PartOfSpeech::Preposition;
        if (p == "pron.") return PartOfSpeech::Pronoun;
        if (p == "conj.") return PartOfSpeech::Conjunction;
        if (p == "interj.") return PartOfSpeech::Interjection;

        return PartOfSpeech::Unknown;
    }

    static const char* posToText(PartOfSpeech pos) {
        switch (pos) {
        case PartOfSpeech::Noun:         return "noun";
        case PartOfSpeech::Pronoun:      return "pronoun";
        case PartOfSpeech::Adjective:    return "adjective";
        case PartOfSpeech::Adverb:       return "adverb";
        case PartOfSpeech::Verb:         return "verb";
        case PartOfSpeech::Preposition:  return "preposition";
        case PartOfSpeech::Conjunction:  return "conjunction";
        case PartOfSpeech::Interjection: return "interjection";
        default:                         return "";
        }
    }

    Dictionary::Dictionary(const char* filename) {
        if (!filename || !filename[0]) {
            m_words = nullptr;
            m_size = 0;
            return;
        }

        std::ifstream fin(filename);
        if (!fin) {
            m_words = nullptr;
            m_size = 0;
            return;
        }

        std::string line;
        size_t count = 0;
        while (std::getline(fin, line)) {
            if (!trim(line).empty())
                ++count;
        }

        if (count == 0) {
            m_words = nullptr;
            m_size = 0;
            return;
        }

        m_words = new Word[count];
        m_size = count;

        fin.clear();
        fin.seekg(0);

        size_t idx = 0;
        while (idx < m_size && std::getline(fin, line)) {
            line = trim(line);
            if (line.empty()) continue;

            std::string w, pos, def;
            if (!parseCSV3(line, w, pos, def)) {
                continue;
            }

            m_words[idx].m_word = unquote(trim(w));
            m_words[idx].m_pos = mapPOS(pos);
            m_words[idx].m_definition = trim(def);

            ++idx;
        }

        if (idx < m_size) {
            Word* smaller = (idx > 0) ? new Word[idx] : nullptr;
            for (size_t i = 0; i < idx; ++i) smaller[i] = m_words[i];
            delete[] m_words;
            m_words = smaller;
            m_size = idx;
        }
    }

    Dictionary::~Dictionary() {
        delete[] m_words;
        m_words = nullptr;
        m_size = 0;
    }

    Dictionary::Dictionary(const Dictionary& other) {
        m_words = nullptr;
        m_size = 0;

        if (other.m_size == 0 || !other.m_words) return;

        m_size = other.m_size;
        m_words = new Word[m_size];
        for (size_t i = 0; i < m_size; ++i) {
            m_words[i] = other.m_words[i];
        }
    }

    Dictionary& Dictionary::operator=(const Dictionary& other) {
        if (this != &other) {
            delete[] m_words;
            m_words = nullptr;
            m_size = 0;

            if (other.m_size > 0 && other.m_words) {
                m_size = other.m_size;
                m_words = new Word[m_size];
                for (size_t i = 0; i < m_size; ++i) {
                    m_words[i] = other.m_words[i];
                }
            }
        }
        return *this;
    }

    Dictionary::Dictionary(Dictionary&& other) noexcept {
        m_words = nullptr;
        m_size = 0;
        *this = std::move(other);
    }

    Dictionary& Dictionary::operator=(Dictionary&& other) noexcept {
        if (this != &other) {
            delete[] m_words;

            m_words = other.m_words;
            m_size = other.m_size;

            other.m_words = nullptr;
            other.m_size = 0;
        }
        return *this;
    }

    void Dictionary::searchWord(const char* word) const {
        if (!word || !word[0]) return;

        bool foundAny = false;
        const std::string target(word);
        const std::string indent(target.size(), ' ');

        for (size_t i = 0; i < m_size; ++i) {
            if (m_words[i].m_word == target) {

                const bool showPos =
                    g_settings.m_verbose &&
                    m_words[i].m_pos != PartOfSpeech::Unknown;

                if (!foundAny) {
                    std::cout << target << " - ";
                }
                else {
                    std::cout << indent << " - ";
                }

                if (showPos) {
                    std::cout << "(" << posToText(m_words[i].m_pos) << ") ";
                }

                std::cout << m_words[i].m_definition << '\n';

                foundAny = true;

                if (!g_settings.m_show_all) return;
            }
        }

        if (!foundAny) {
            std::cout << "Word '" << target << "' was not found in the dictionary.\n";
        }
    }

}
