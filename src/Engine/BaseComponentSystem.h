#pragma once
#include "ComponentSystem.h"
#include "InheritanceTree.h"

namespace xihad { namespace ngn
{
	/// ���ü̳��������ϵͳ
	class BaseComponentSystem : public ComponentSystem
	{
	public:
		/**
		 * @param hierarchy ��ι�ϵ����� #create() ����ʵ�ֶ�Ӧ�������������ĳ�����ͣ���ô
		 *					#create() �������뷵��һ���ǿ����
		 */
		BaseComponentSystem(const InheritanceTree& hierarchy);

		virtual InheritancePath hierarchy( const std::string& compName ) override;

	private:
		const InheritanceTree& mHierarchyTree;
	};
}}
