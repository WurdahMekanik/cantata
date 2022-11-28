/*
 * Cantata
 *
 * Copyright (c) 2011-2022 Craig Drummond <craig.p.drummond@gmail.com>
 *
 */
/* This file is part of Clementine.
   Copyright 2010, David Sansome <me@davidsansome.com>

   Clementine is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Clementine is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Clementine.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef ULTIMATELYRICSPROVIDER_H
#define ULTIMATELYRICSPROVIDER_H

#include "mpd-interface/song.h"
#include <QObject>
#include <QPair>
#include <QStringList>
#include <QHash>
#include <QMap>
#include <QRegularExpression>

class NetworkJob;

class UltimateLyricsProvider : public QObject {
    Q_OBJECT

public:
    static void enableDebug() { debugEnabled = true; }

    UltimateLyricsProvider();
    ~UltimateLyricsProvider() override;

    typedef QPair<QString, QString> RuleItem;
    typedef QList<RuleItem> Rule;
    typedef QPair<QString, QString> UrlFormat;

    void setName(const QString &n) { name = n; }
    void setUrl(const QString &u) { url = u; }
    void setCharset(const QString &c) { charset = c; }
    void setRelevance(int r) { relevance = r; }
    void addUrlFormat(const QString &replace, const QString &with) { urlFormats << UrlFormat(replace, with); }
    void addExtractRule(const Rule &rule) { extractRules << rule; }
    void addExcludeRule(const Rule &rule) { excludeRules << rule; }
    void addInvalidIndicator(const QString &indicator) { invalidIndicators << indicator; }
    QString getName() const { return name; }
    QString displayName() const;
    int getRelevance() const { return relevance; }
    void fetchInfo(int id, Song metadata, bool removeThe=false);
    bool isEnabled() const { return enabled; }
    void setEnabled(bool e) { enabled = e; }
    void abort();

Q_SIGNALS:
    void lyricsReady(int id, const QString &data);

private Q_SLOTS:
    void wikiMediaSearchResponse();
    void wikiMediaLyricsFetched();
    void lyricsFetched();

private:
    static QString extract(const QString &source, const QString &begin, const QString &end, bool isTag=false);
    static QString extractXmlTag(const QString &source, const QString &tag);
    static QString exclude(const QString &source, const QString &begin, const QString &end);
    static QString excludeXmlTag(const QString &source, const QString &tag);
    static void applyExtractRule(const UltimateLyricsProvider::Rule &rule, QString &content, const Song &song);
    static void applyExcludeRule(const UltimateLyricsProvider::Rule &rule, QString &content, const Song &song);
    static bool tryWithoutThe(const Song &s);
    static QString doTagReplace(QString str, const Song &song);

    void doUrlReplace(const QString &tag, const QString &value, QString &u) const;

    inline static bool debugEnabled = false;
    inline static const QString constArtistArg = u"{Artist}"_qs;
    inline static const QString constArtistLowerArg = u"{artist}"_qs;
    inline static const QString constArtistLowerNoSpaceArg = u"{artist2}"_qs;
    inline static const QString constArtistFirstCharArg = u"{a}"_qs;
    inline static const QString constAlbumArg = u"{Album}"_qs;
    inline static const QString constAlbumLowerArg = u"{album}"_qs;
    inline static const QString constAlbumLowerNoSpaceArg = u"{album2}"_qs;
    inline static const QString constTitleLowerArg = u"{title}"_qs;
    inline static const QString constTitleArg = u"{Title}"_qs;
    inline static const QString constTitleCaseArg = u"{Title2}"_qs;
    inline static const QString constYearArg = u"{year}"_qs;
    inline static const QString constTrackNoArg = u"{track}"_qs;
    inline static const QString constThe = u"The "_qs;
        // ಠ_ಠ
    inline static const QRegularExpression tag_regexp = QRegularExpression("<(\\w+).*>");

    bool enabled;
    QHash<NetworkJob *, int> requests;
    QMap<int, Song> songs;
    QString name;
    QString url;
    QString charset;
    int relevance;
    QList<UrlFormat> urlFormats;
    QList<Rule> extractRules;
    QList<Rule> excludeRules;
    QStringList invalidIndicators;
};

#endif // ULTIMATELYRICSPROVIDER_H
