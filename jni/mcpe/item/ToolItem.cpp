#include "ToolItem.h"

#include <algorithm>

#include "enchant/EnchantUtils.h"
#include "../client/resources/I18n.h"
#include "../block/Block.h"

ToolItem::ToolItem(const std::string& name, short id, float damage, Item::Tier tier, std::vector<Block*> blocks)
    : Item(name, id) {
    
    toolMaterial = &tier;
    validBlocks = blocks;
    _maxStackSize = 1;
    setMaxDamage(tier.maxUses);
    efficiencyOnProperMaterial = tier.efficiencyOnProperMaterial;
    damageVsEntity = damage + tier.damageVsEntity;
}

int ToolItem::getAttackDamage() {
    return damageVsEntity;
}

bool ToolItem::isHandEquipped() const {
    return true;
}

std::string ToolItem::appendFormattedHovertext(const ItemInstance& item, Level& level, std::string& s, bool b) const {
    return Item::appendFormattedHovertext(item, level, s, b) + "\n\n+5 " + I18n::get("attribute.name.generic.attackDamage"); // todo
}

bool ToolItem::isValidRepairItem(const ItemInstance& item, const ItemInstance& repairItem) {
    return toolMaterial->getTierItem().item == repairItem.item ? true : Item::isValidRepairItem(item, repairItem);
}

int ToolItem::getEnchantValue() const {
    return toolMaterial->enchantability;
}

float ToolItem::getDestroySpeed(ItemInstance* item, const Block* block) {
    if(block && std::find(validBlocks.begin(), validBlocks.end(), block) != validBlocks.end())
        return efficiencyOnProperMaterial + destroySpeedBonus(item);
    return 1.0F;
}

bool ToolItem::mineBlock(ItemInstance* item, BlockID blockId, int x, int y, int z, Entity* miner) {
    if(Block::mBlocks[blockId.id] && Block::mBlocks[blockId.id]->canHurtAndBreakItem())
        item->hurtAndBreak(1, miner);
    return true;
}