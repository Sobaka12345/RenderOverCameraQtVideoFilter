#include "CustomVideoFilter.h"

#include "FrameHelper.h"

CustomVideoFilterRunnable::CustomVideoFilterRunnable(CustomVideoFilter * filter) :
    m_filter(filter),
    m_fbo(0)
{
    qDebug() << m_program.addShaderFromSourceFile(QOpenGLShader::Vertex, ":/vertex.vsh");

    qDebug() << m_program.addShaderFromSourceFile(QOpenGLShader::Fragment, ":/fragment.fsh");

    qDebug() << m_program.link();

    qDebug() << m_program.bind();

    QImage image(":/rei.jpg");

    glGenTextures(1, &buf_tex);
    glBindTexture(GL_TEXTURE_2D, buf_tex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width(), image.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, image.bits());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}


QVideoFrame CustomVideoFilterRunnable::run(QVideoFrame *input, const QVideoSurfaceFormat &surfaceFormat, QVideoFilterRunnable::RunFlags flags)
{
    QSize size = input->size();

    if(input->handleType() != QAbstractVideoBuffer::GLTextureHandle)
    {
        return *input;
    }

    QOpenGLFunctions * f = QOpenGLContext::currentContext()->functions();

    if(m_fbo == 0)
    {
        f->glGenFramebuffers(1, &m_fbo);
        f->glGenTextures(1, &m_tex);
        f->glGenRenderbuffers(1, &m_rbo);

        f->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
        f->glViewport(0, 0, size.width(), size.height());

        f->glBindTexture(GL_TEXTURE_2D, m_tex);
        f->glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, size.width(), size.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        f->glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tex, 0);


        f->glGenRenderbuffers(1,&m_rbo);
        f->glBindRenderbuffer(GL_RENDERBUFFER, m_rbo);
        f->glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, size.width(), size.height());

        if(f->glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            qDebug() << "Ploho";


        f->glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    copyTexture(input->handle().toUInt(), input->size());

    f->glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);
    f->glClearColor(1,0,0,1);
    f->glClear(GL_DEPTH_BUFFER_BIT);

    m_program.bind();
    f->glActiveTexture(GL_TEXTURE0);
    f->glBindTexture(GL_TEXTURE_2D, buf_tex);
    m_program.setUniformValue("texture", 0);
    drawQuad(f);

    return frameFromTexture(m_tex, size, input->pixelFormat());
}

void CustomVideoFilterRunnable::drawQuad(QOpenGLFunctions *f)
{
    GLfloat data[] = {
        -1.0f, -1.0f, 0,
        -1.0f, 1.0f, 0,
        1.0f, 0.5f, 0,
        -1.0, -1.0, 0,
        1.0, 1.0, 0,
        1.0f, -1.0f, 0,
    };

    f->glVertexAttribPointer(0,3,GL_FLOAT, GL_FALSE, 0, data);
    f->glEnableVertexAttribArray(0);
    f->glDrawArrays(GL_TRIANGLES, 0, 3);
    f->glDisableVertexAttribArray(0);
}

void CustomVideoFilterRunnable::copyTexture(GLuint src, QSize size)
{
    GLuint fboId = 0;
    glGenFramebuffers(1, &fboId);
    glBindFramebuffer(GL_FRAMEBUFFER, fboId);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, src, 0);

    glBindTexture(GL_TEXTURE_2D, m_tex);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, size.width(), size.height(), 0);
    glDeleteFramebuffers(1, &fboId);
}

