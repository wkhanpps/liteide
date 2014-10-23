/**************************************************************************
** This file is part of LiteIDE
**
** Copyright (c) 2011-2014 LiteIDE Team. All rights reserved.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License as published by the Free Software Foundation; either
** version 2.1 of the License, or (at your option) any later version.
**
** This library is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
** Lesser General Public License for more details.
**
** In addition, as a special exception,  that plugins developed for LiteIDE,
** are allowed to remain closed sourced and can be distributed under any license .
** These rights are included in the file LGPL_EXCEPTION.txt in this package.
**
**************************************************************************/
// Module: liteeditorapi.h
// Creator: visualfc <visualfc@gmail.com>

#ifndef __LITEEDITORAPI_H__
#define __LITEEDITORAPI_H__

#include "liteapi/liteapi.h"
#include <QTextCursor>
#include <QTextBlock>
#include <QCompleter>
#include <QStandardItem>

namespace TextEditor {
class SyntaxHighlighter;
}

namespace LiteApi {

class IWordApi
{
public:
    virtual ~IWordApi() {}
    virtual QString mimeType() const = 0;
    virtual QStringList apiFiles() const = 0;
    virtual bool loadApi() = 0;
    virtual QStringList wordList() const = 0;
    virtual QStringList expList() const = 0;
    virtual void appendExp(const QStringList &list) = 0;
};

class IWordApiManager : public IManager
{
    Q_OBJECT
public:
    IWordApiManager(QObject *parent = 0) : IManager(parent) {}
    virtual void addWordApi(IWordApi *wordApi) = 0;
    virtual void removeWordApi(IWordApi *wordApi) = 0;
    virtual IWordApi *findWordApi(const QString &mimeType) = 0;
    virtual QList<IWordApi*> wordApiList() const = 0;
};

class ISnippet
{
public:
    virtual ~ISnippet() {}
    virtual QString trigger() const = 0;
    virtual void setTrigger(const QString &trigger) = 0;
    virtual QString content() const = 0;
    virtual void setContent(const QString &content) = 0;
};

class ISnippetList
{
public:
    virtual ~ISnippetList() {}
    virtual QString mimeType() const = 0;
    virtual bool load() = 0;
    virtual QList<ISnippet*> findSnippet(const QString &trigger, Qt::CaseSensitivity cs = Qt::CaseInsensitive) const = 0;
    virtual QList<ISnippet*> snippetList() const = 0;
};

class ISnippetsManager : public IManager
{
public:
    ISnippetsManager(QObject *parent = 0) : IManager(parent) {}
    virtual void addSnippetList(ISnippetList *snippets) = 0;
    virtual void removeSnippetList(ISnippetList *snippets) = 0;
    virtual ISnippetList *findSnippetList(const QString &mimeType) = 0;
    virtual QList<ISnippetList*> allSnippetList() const = 0;
};

class ICompleter : public QObject
{
    Q_OBJECT
public:
    ICompleter(QObject *parent): QObject(parent) {}
    virtual void setEditor(QPlainTextEdit *editor) = 0;
    virtual QStandardItem *findRoot(const QString &name) = 0;
    virtual void clearChildItem(QStandardItem *root) = 0;
    virtual void appendChildItem(QStandardItem *root,QString name,const QString &kind, const QString &info,const QIcon &icon, bool temp) = 0;
    virtual bool appendItem(const QString &name,const QIcon &icon, bool temp) = 0;
    virtual bool appendItemEx(const QString &name,const QString &kind, const QString &info,const QIcon &icon, bool temp) = 0;
    virtual void appendItems(QStringList items, const QString &kind, const QString &info,const QIcon &icon, bool temp) = 0;
    virtual void clearItemChilds(const QString &name) = 0;
    virtual void clearTemp() = 0;
    virtual void clear() = 0;
    virtual void setSearchSeparator(bool b) = 0;
    virtual bool searchSeparator() const = 0;
    virtual void setExternalMode(bool b) = 0;
    virtual bool externalMode() const = 0;
    virtual void setCaseSensitivity(Qt::CaseSensitivity caseSensitivity) = 0;
    virtual void setCompletionPrefix(const QString &prefix) = 0;
    virtual QString completionPrefix() const = 0;
    virtual void showPopup() = 0;
    virtual QAbstractItemView *popup() const = 0;
    virtual QModelIndex currentIndex() const = 0;
    virtual QString currentCompletion() const = 0;
    virtual QAbstractItemModel *completionModel() const = 0;
    virtual bool startCompleter(const QString &completionPrefix) = 0;
    virtual void updateCompleterModel() = 0;
signals:
    void prefixChanged(QTextCursor,QString,bool force);
    void wordCompleted(const QString &func, const QString &kind, const QString &info);
};

// priopity by type value
class IEditorMarkTypeManager : public IManager
{
    Q_OBJECT
public:
    IEditorMarkTypeManager(QObject *parent = 0) : IManager(parent) {}
    virtual void registerMark(int type, const QIcon &icon) = 0;
    virtual QList<int> markTypeList() const = 0;
};

class IEditorMark : public QObject
{
    Q_OBJECT
public:
    IEditorMark(QObject *parent) : QObject(parent) {}
    virtual void addMark(int line, int type) = 0;
    virtual void removeMark(int line, int type) = 0;
    virtual QList<int> markList(int type) const = 0;
    virtual QList<int> lineTypeList(int line) const = 0;
signals:
    void markChanged();
};

enum EditorNaviagteType{
    EditorNavigateNormal = 1,
    EditorNavigateWarning = 2,
    EditorNavigateError = 4,
    EditorNavigateReload = 8,
    EditorNavigateBad = EditorNavigateWarning|EditorNavigateError
};

enum ExtraSelectionKind {
    CurrentLineSelection,
    ParenthesesMatchingSelection,
    LinkSelection,
};

struct Link
{
    Link(const QString &fileName = QString(), int line = 0, int column = 0)
        : linkTextStart(-1)
        , linkTextEnd(-1)
        , targetFileName(fileName)
        , targetLine(line)
        , targetColumn(column)
    {}

    bool hasValidTarget() const
    { return !targetFileName.isEmpty(); }

    bool hasValidLinkText() const
    { return linkTextStart != linkTextEnd; }

    bool operator==(const Link &other) const
    { return linkTextStart == other.linkTextStart && linkTextEnd == other.linkTextEnd; }

    int linkTextStart;
    int linkTextEnd;

    QString targetFileName;
    int targetLine;
    int targetColumn;
};

class ITextLexer
{
public:
    ITextLexer()
    {}
    virtual ~ITextLexer()
    {}
    virtual bool isInComment(const QTextCursor &cursor) const = 0;
    virtual bool isInString(const QTextCursor &cursor) const = 0;
    virtual bool isInEmptyString(const QTextCursor &cursor) const = 0;
    virtual bool isInStringOrComment(const QTextCursor &cursor) const = 0;
    virtual bool isCanCodeCompleter(const QTextCursor &cursor) const = 0;
    virtual bool isCanAutoCompleter(const QTextCursor &cursor) const = 0;
    virtual int startOfFunctionCall(const QTextCursor &cursor) const = 0;
    virtual QString fetchFunctionTip(const QString &func, const QString &kind, const QString &info) = 0;
    virtual bool fetchFunctionArgs(const QString &str, int &argnr, int &parcount) = 0;
};

class BaseTextLexer : public ITextLexer
{
public:
    BaseTextLexer() : m_bAC(true), m_bCC(true)
    {
    }
    virtual bool isInComment(const QTextCursor &/*cursor*/) const {
        return false;
    }
    virtual bool isInString(const QTextCursor &/*cursor*/) const {
        return false;
    }
    virtual bool isInEmptyString(const QTextCursor &/*cursor*/) const {
        return false;
    }
    virtual bool isInStringOrComment(const QTextCursor &/*cursor*/) const {
        return false;
    }
    virtual bool isCanCodeCompleter(const QTextCursor &/*cursor*/) const {
        return m_bCC;
    }
    virtual bool isCanAutoCompleter(const QTextCursor &/*cursor*/) const {
        return m_bAC;
    }
    virtual void setCanCodeCompleter(bool b) {
        m_bCC = b;
    }
    virtual void setCanAutoCompleter(bool b) {
        m_bAC = b;
    }
    virtual int startOfFunctionCall(const QTextCursor &/*cursor*/) const {
        return -1;
    }
    virtual QString fetchFunctionTip(const QString &/*func*/, const QString &/*kind*/, const QString &/*info*/)
    {
        return QString();
    }
    virtual bool fetchFunctionArgs(const QString &/*str*/, int &/*argnr*/, int &/*parcount*/)
    {
        return false;
    }
protected:
    bool m_bAC;
    bool m_bCC;
};

class ILiteEditor : public ITextEditor
{
    Q_OBJECT
public:
    ILiteEditor(QObject *parent = 0) : ITextEditor(parent) {}
    virtual QTextDocument* document() const = 0;
    virtual void setCompleter(ICompleter *complter) = 0;
    virtual void setEditorMark(IEditorMark *mark) = 0;
    virtual void setTextLexer(ITextLexer *lexer) = 0;
    virtual void setSpellCheckZoneDontComplete(bool b) = 0;
    virtual void insertNavigateMark(int line, EditorNaviagteType type, const QString &msg, const char *tag) = 0;
    virtual void clearNavigateMarak(int line) = 0;
    virtual void clearAllNavigateMarks() = 0;
    virtual void clearAllNavigateMark(EditorNaviagteType types, const char *tag = "") = 0;
    virtual void setNavigateHead(EditorNaviagteType type, const QString &msg) = 0;
    virtual void showLink(const Link &link) = 0;
    virtual void clearLink() = 0;
signals:
    void updateLink(const QTextCursor &cursor);
};

inline ILiteEditor *getLiteEditor(IEditor *editor)
{
    if (editor && editor->extension()) {
        return findExtensionObject<ILiteEditor*>(editor->extension(),"LiteApi.ILiteEditor");
    }
    return 0;
}

class IHighlighterFactory : public QObject
{
    Q_OBJECT
public:
    IHighlighterFactory(QObject *parent) : QObject(parent)
    {}
    virtual QStringList mimeTypes() const = 0;
    virtual TextEditor::SyntaxHighlighter* create(QTextDocument *doc, const QString &mimeType) = 0;
};

class IHighlighterManager :public IManager
{
    Q_OBJECT
public:
    IHighlighterManager(QObject *parent) : IManager(parent)
    {}
    virtual void addFactory(IHighlighterFactory *factroy) = 0;
    virtual void removeFactory(IHighlighterFactory *factory) = 0;
    virtual QList<IHighlighterFactory*> factoryList() const = 0;
    virtual QStringList mimeTypeList() const = 0;
    virtual IHighlighterFactory *findFactory(const QString &mimeType) const = 0;
};

inline IHighlighterManager *getHighlighterManager(LiteApi::IApplication *app)
{
    return static_cast<IHighlighterManager*>(app->extension()->findObject("LiteApi.IHighlighterManager"));
}

inline QString wordUnderCursor(QTextCursor tc, bool *moveLeft = 0)
{
    QString text = tc.block().text();
    int pos = tc.positionInBlock();
    if (pos > 0 && pos < text.length()) {
        QChar ch = text.at(pos-1);
        if (ch.isLetterOrNumber() || ch == '_') {
            tc.movePosition(QTextCursor::Left);
            if (moveLeft) {
                *moveLeft = true;
            }
        }
    }
    tc.select(QTextCursor::WordUnderCursor);
    return tc.selectedText();
}

inline void selectWordUnderCursor(QTextCursor &tc, bool *moveLeft = 0)
{
    QString text = tc.block().text();
    int pos = tc.positionInBlock();
    if (pos > 0 && pos < text.length()) {
        QChar ch = text.at(pos-1);
        if (ch.isLetterOrNumber() || ch == '_') {
            tc.movePosition(QTextCursor::Left);
            if (moveLeft) {
                *moveLeft = true;
            }
        }
    }
    tc.select(QTextCursor::WordUnderCursor);
}


} //namespace LiteApi


#endif //__LITEEDITORAPI_H__

