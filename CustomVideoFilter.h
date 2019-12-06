#ifndef CUSTOMVIDEOFILTER_H
#define CUSTOMVIDEOFILTER_H

#include <QAbstractVideoFilter>
#include <QVideoFilterRunnable>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <QOpenGLContext>


class CustomVideoFilter;

class CustomVideoFilterRunnable : public QVideoFilterRunnable
{
public:
    QVideoFrame run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, RunFlags flags);
    CustomVideoFilterRunnable(CustomVideoFilter * filter = nullptr);
    virtual ~CustomVideoFilterRunnable() {}
private:
    CustomVideoFilter * m_filter;
    QOpenGLShaderProgram m_program;
    GLuint m_fbo, m_tex, buf_tex, m_rbo;

    void drawQuad(QOpenGLFunctions * f);
    void copyTexture(GLuint src, QSize size);
};

class CustomVideoFilter : public QAbstractVideoFilter
{
public:
    QVideoFilterRunnable * createFilterRunnable() override { return new CustomVideoFilterRunnable(this); }

signals:
    void finished(QObject * result);
};

#endif // CUSTOMVIDEOFILTER_H
