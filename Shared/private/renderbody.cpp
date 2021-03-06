#include "renderbody.h"

const QString RenderBody::imagePath = ":/bodies/";
const QString RenderBody::maskPath = ":/bodies-mask/";

RenderBody::RenderBody() :
    Body()
{
    this->spriteGraphicsItem = NULL;
}


RenderBody::RenderBody(QString assetPath) :
    Body()
{
    this->spriteGraphicsItem = NULL;
    this->assetPath = assetPath;
}

bool RenderBody::createGraphic(QGraphicsScene* scene)
{
    this->scene = scene;

    // Create QImages so they can be manipulated pixel by pixel
    QImage image(this->imagePath + this->assetPath);
    QImage mask(this->maskPath + this->assetPath);

    if (image.isNull())
    {
        QMessageBox errorBox;
        errorBox.setText("There was a problem loading asset: " + this->imagePath + this->assetPath);
        errorBox.exec();
        return false;
    }

    // Not a fatal error
    if (mask.isNull())
    {
        QMessageBox errorBox;
        errorBox.setText("There was a problem loading asset: " + this->maskPath + this->assetPath);
        errorBox.exec();
    }

    // Apply image filters
    if (!mask.isNull())
    {
        this->applyMask(&image, &mask);
    }

    // Create the pixmap and graphics item for rendering
    QPixmap pixmap;
    pixmap.convertFromImage(image);

    if (pixmap.isNull())
    {
        QMessageBox errorBox;
        errorBox.setText("There was a problem creating a pixmap for asset: " + this->imagePath + this->assetPath);
        errorBox.exec();
        return false;
    }
    else
    {
        this->spriteGraphicsItem = new QGraphicsPixmapItem(pixmap);
        this->spriteGraphicsItem->setTransformOriginPoint(pixmap.width() / 2, pixmap.height() / 2);
        return true;
    }
}

void RenderBody::applyMask(QImage* image, QImage* mask)
{
    // Image must have a size, and the mask must be the same size
    if (image->size() != QSize(0, 0) && image->size() == mask->size())
    {
        // Colors in mask to replace in image
        // Red, Blue, Green
        QColor primaryMask;
        primaryMask.setRed(255);
        QColor secondaryMask;
        secondaryMask.setBlue(255);
        QColor tertiaryMask;
        tertiaryMask.setGreen(255);

        // Colors to put in mask
        QColor primary;
        QColor secondary;
        QColor tertiary;

        // Set colors
        primary.setRed(this->primaryMaskRed);
        primary.setGreen(this->primaryMaskGreen);
        primary.setBlue(this->primaryMaskBlue);

        secondary.setRed(this->secondaryMaskRed);
        secondary.setGreen(this->secondaryMaskGreen);
        secondary.setBlue(this->secondaryMaskBlue);

        tertiary.setRed(this->tertiaryMaskRed);
        tertiary.setGreen(this->tertiaryMaskGreen);
        tertiary.setBlue(this->tertiaryMaskBlue);

        // Loop through each pixel in the images
        for (int x = 0; x < mask->size().width(); x++)
        {
            for (int y = 0; y < mask->size().height(); y++)
            {
                // For red in mask, replace with primary
                if (mask->pixel(x, y) == primaryMask.rgb())
                {
                    image->setPixel(x, y, primary.rgb());
                }
                // For blue in mask, replace with secondary
                else if (mask->pixel(x, y) == secondaryMask.rgb())
                {
                    image->setPixel(x, y, secondary.rgb());
                }
                // For green in mask, replace with tertiary
                else if (mask->pixel(x, y) == tertiaryMask.rgb())
                {
                    image->setPixel(x, y, tertiary.rgb());
                }
            }
        }
    }
}

void RenderBody::tick(double deltaTime)
{
    // Call the parent version of tick
    Body::tick(deltaTime);
}

QGraphicsPixmapItem* RenderBody::getGraphicsItem()
{
    // Get the graphics item
    return this->spriteGraphicsItem;
}

void RenderBody::setAssetPath(QString assetPath)
{
    this->assetPath = assetPath;
}

QString RenderBody::getAssetPath()
{
    return this->assetPath;
}

void RenderBody::read(const QJsonObject &json)
{
    // Read in a JSON object
    Body::read(json);

    this->assetPath = json["asset"].toString();
}

void RenderBody::write(QJsonObject &json) const
{
    // Write to a JSON object
    Body::write(json);

    json["asset"] = this->assetPath;
    json["type"] = QString("RenderBody");
}

Vector RenderBody::getScreenPosition()
{
    return Vector(
                this->spriteGraphicsItem->x() + (this->spriteGraphicsItem->boundingRect().width()/2),
                this->spriteGraphicsItem->y() + (this->spriteGraphicsItem->boundingRect().height()/2)
                 );
}
