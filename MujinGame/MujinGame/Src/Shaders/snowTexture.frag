    #version 330

    in vec2 fragmentPosition;
    in vec4 fragmentColor;
    in vec2 fragmentUV;

    out vec4 color;

    uniform sampler2D texture_sampler;

    void main() {
        
        vec4 textureColor = texture(texture_sampler, fragmentUV);

        float stripeWidth = 5.0 / 512.0; // The normalized width of the stripe
        float stripePeriod = 32.0 / 512.0; // The normalized distance between stripes
        bool isStripe = mod(fragmentUV.y, stripePeriod) < stripeWidth;
        bool isInLeftHalf = fragmentUV.x < 0.5;

        if (isStripe && isInLeftHalf) {
            textureColor = vec4(1.0, 1.0, 1.0, textureColor.a);
        }

        color = fragmentColor * textureColor;

    }