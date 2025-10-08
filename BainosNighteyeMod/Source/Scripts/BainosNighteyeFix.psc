Scriptname BainosNighteyeFix extends ActiveMagicEffect

; Native function - implemented in C++
Function toggle() Global Native

; This function runs when the player uses the Night Eye Power (default Z key).
Event OnEffectStart(Actor akTarget, Actor akCaster)

    Debug.Trace("Bainos Magic effect was started on " + akTarget)
    Debug.MessageBox("Bainos Magic effect was started on " + akTarget)
    string str1 = SkyMessage.Show("Bainos Magic effect was started", "Ok", "Cancel")

    ; Verify the effect is cast by the player
    if akTarget == Game.GetPlayer()

        ; --- YOUR CUSTOM LOGIC GOES HERE ---

        ; Example: Display a message and toggle the actual visual effect.
        ; Note: Actual visual Night Eye requires calling a separate effect or function.
        ; For now, we'll just confirm activation.

        Debug.MessageBox("Baino's Night Eye Script Executed.")
        Debug.Trace("Baino's Night Eye Script Executed.")
        Debug.MessageBox("Baino's Night Eye Script Executed.")
        string str2 = SkyMessage.Show("Baino's Night Eye Script Executed.", "Ok", "Cancel")

        ; In a full implementation, you would use this script to:
        ; 1. Check if the visual effect is already active on the player.
        ; 2. If active, use akCaster.RemoveSpell(VisualFXSpell) to turn it off.
        ; 3. If inactive, use akCaster.Cast(VisualFXSpell, akCaster) to turn it on.

    endif
EndEvent